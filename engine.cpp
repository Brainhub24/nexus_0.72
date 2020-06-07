// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

CNexEngine nexEngine;

void CNexEngine::prev_CG_Init() {
	#ifdef NEX_LOGGING
		nexUtils.log("vmMain: CG_INIT");
	#endif

	if (nex.mod->type == MOD_ETPRO)
		nexEtpro.redirectSyscall();

	// remove ET.exe detours because pb detects it :<
	nexHook.hookET(false);

	nexEngine.registerMedias();
}

void CNexEngine::post_CG_Init() {
	orig_syscall(CG_R_REGISTERFONT, "ariblk", 27, &nex.media.mainFont);

	// Get glconfig
	orig_syscall(CG_GETGLCONFIG, &nex.cgs_glconfig);
	nex.cgs_screenXScale = nex.cgs_glconfig.vidWidth / 640.0f;
	nex.cgs_screenYScale = nex.cgs_glconfig.vidHeight / 480.0f;

	// Init all cg_entities ptrs
	int	entityCount = 0;
	DWORD cg_ent_offset = (DWORD) GetProcAddress(sNex.cgHandle, "cg_entities");
	if (!cg_ent_offset)
		cg_ent_offset = (DWORD) sNex.cgHandle + nex.mod->cg_entities;
	for (; entityCount < MAX_GENTITIES; entityCount++) {
		DWORD offset = cg_ent_offset + (entityCount * nex.mod->centity_t_size);
		nex.cg_entities[entityCount].currentState = (entityState_t *) (offset + CG_ENTITIES_CURRENTSTATE_OFFSET);
		nex.cg_entities[entityCount].currentValid = (qboolean *) (offset + CG_ENTITIES_CURRENTVALID_OFFSET);
	}

	nexHook.unreferenceBadPk3s();

	if (nex.mod->type == MOD_ETPRO)
		nexEtpro.redirectSyscall();
}

void CNexEngine::prev_CG_DrawActiveFrame() {
	#ifdef NEX_VERBOSE
		nexUtils.log("vmMain: CG_DRAW_ACTIVE_FRAME | time: %i | frametime: %i", nex.cg_time, nex.cg_frametime);
	#endif

	// some	of our functions are using snapshot info
	if (!nex.cg_snap)
		return;

	// same goes for gameState
	if (!nex.cgs_gameState)
		return;

	nexVisuals.setColors();
	nexEngine.setMuzzle();
}

void CNexEngine::post_CG_DrawActiveFrame() {
	// some	of our functions are using snapshot info
	if (!nex.cg_snap)
		return;

	// same goes for gameState
	if (!nex.cgs_gameState)
		return;

	// don't draw stuph in intermission
	if (nex.cgs_gamestate != GS_INTERMISSION) {
		if (nexValue[VAR_ESP])
			nexVisuals.drawEspEntities();
		if (nexValue[VAR_ADVERT])
			nexVisuals.drawAdvert();
		if (nexValue[VAR_SPAWNTIMER])
			nexVisuals.drawSpawnTimer();
		if (nexValue[VAR_RADAR]) // make it resizable later
			nexVisuals.drawRadar(100);

		// menu - should be the last visual being drawn
		static int lastMenuToggle = 0;
		if (((nex.cg_time - lastMenuToggle) > NEX_MENU_KEY_DELAY) && GetAsyncKeyState(NEX_MENU_KEY)) {
			lastMenuToggle = nex.cg_time;
			nex.isMeunOpen = !nex.isMeunOpen;
		}
		if (nex.isMeunOpen)
			nexMenu.drawMenu();
	}

	nexAimbot.doAimbot();

	// End of frame so delete all nexEntities/refEntities.
	memset(nex.entities, 0, sizeof(nex.entities));
	memset(nex.refEntities, 0, sizeof(nex.refEntities));
	nex.refEntitiesCount = 0;
}

void CNexEngine::CG_Shutdown() {
	// remove detours
	nexHook.hookCG(false);

	// save settings
	nexMenu.writeConfig();

	// reset main struct
	nexUtils.resetNex();

	nexHook.hookET(true);
}

int CNexEngine::getSpawntime(bool enemySpawn) {
	int team = nex.client[nex.cg_snap->ps.clientNum].team;
	int limbotime = 0;

	// Reverse the value from client[myself].team to make the function return the opposite spawntimer
    if (enemySpawn) {
		if (team == TEAM_AXIS) {
			team = TEAM_ALLIES;
			limbotime = nex.cg_bluelimbotime;
		} else if (team == TEAM_ALLIES) {
			team = TEAM_AXIS;
			limbotime = nex.cg_redlimbotime;
		} else
			return -1;
    } else {
		if (team == TEAM_AXIS) {
			limbotime = nex.cg_redlimbotime;
		} else if (team == TEAM_ALLIES) {
			limbotime = nex.cg_bluelimbotime;
		} else
			return -1;
    }
    
    // Sanity check
    if (limbotime == 0) {
		#ifdef NEX_LOGGING
			nexUtils.log("WARNING spawntimer: can't get spawntime for team %i", team);
		#endif
    	return -1;
    }

    return (int)(1 + (limbotime - ((nex.cgs_aReinfOffset[team] + nex.cg_time - nex.cgs_levelStartTime) % limbotime)) * 0.001f);
}

void CNexEngine::CG_R_AddRefEntityToScene(refEntity_t *re) {
	nexEntity_t*	entity = &nex.entities[re->entityNum];
	int				entityNum = re->entityNum;

	// valid entity =D
	entity->isValid = true;

	// Backup refEntity for spycam
	memcpy(&nex.refEntities[nex.refEntitiesCount++], re, sizeof(refEntity_t));

	if (IS_PLAYER_VALID(entityNum) && !IS_PLAYER_DEAD(entityNum)) {
		// head entity
		if ((re->hModel == nex.media.hHead) || (re->hModel == nex.media.hGlasses)) {
			// Skip already defined head entity (hud head)
			if (!VectorCompare(vec3_origin, entity->head))
				return;

			// Backup head stuff
			VectorCopy(re->origin, entity->head);
			memcpy(entity->headAxis, re->axis, sizeof(vec3_t) * 3);

			// Add some vector Z for better esp display
			vec3_t tmpVector;
			VectorCopy(entity->head, tmpVector);
			tmpVector[ROLL] += 20.0f;
			entity->isInScreen = nexDraw.worldToScreen(tmpVector, &entity->screenX, &entity->screenY);

			// Apply vecz
			nexAimbot.doVecZ(entityNum);

			// If head visible
			if (nexEngine.isVisible(entity->head)) {
				entity->isVisible = true;
				entity->isHeadVisible = true;
			}

			// add chams
			if (nexValue[VAR_CHAMS])
				nexVisuals.addChams(re, entity->isVisible);

			nexVisuals.addEspColors(entityNum, entity->isVisible);

		} else if (re->torsoFrameModel) { // body
			// Backup bodyRefEnt for tag grabbing
			memcpy(&nex.cg_entities[entityNum].pe_bodyRefEnt, re, sizeof(refEntity_t));

			// Find a visible body part
			vec3_t target;
			if (nexAimbot.getVisibleModelBodyTag(entityNum, &target)) {
				VectorCopy(target, entity->bodyPart);
				entity->isVisible = true;
			}

			// add chams
			if (nexValue[VAR_CHAMS])
				nexVisuals.addChams(re, entity->isVisible);

			nexVisuals.addEspColors(entityNum, entity->isVisible);
		} else {
			// Not body or head
			return;
		}
	} else if (IS_MISSILE(entityNum)) {
		entity->isValid = true;
		entity->isVisible = nexEngine.isVisible(re->origin);
	} else {
		return;
	}

	if (nexValue[VAR_WALLHACK])
		re->renderfx |= RF_DEPTHHACK | RF_NOSHADOW;
}

void CNexEngine::CG_S_UpdateEntityPosition(int entityNum, const vec3_t origin) {
	nexEntity_t* entity = &nex.entities[entityNum];
	entity->isValid = true;

	if (IS_PLAYER_VALID(entityNum) || IS_MISSILE(entityNum)) {
		VectorCopy(origin, entity->origin);
		entity->distance = VectorDistance(nex.cg_refdef.vieworg, entity->origin);

		if (entityNum > MAX_CLIENTS)
			entity->isInScreen = nexDraw.worldToScreen(entity->origin, &entity->screenX, &entity->screenY);
	}
}

bool CNexEngine::CG_R_AddPolyToScene(qhandle_t shader, polyVert_t *polyVert) {
	if (nexValue[VAR_SMOKE] == SMOKE_TRANSPARENT) {
		if (shader == nex.media.smokePuffshader) {
			int count = 0;
			for (; count < 4; count ++)
				polyVert[count].modulate[3] = 0x20;
		}
	} else if (nexValue[VAR_SMOKE] == SMOKE_REMOVE) {
		return false;
    }
	return true;
}

bool CNexEngine::CG_R_RenderScene(refdef_t *refdef) {
	char fov[8];
	float x;

	// Check if the scene is the mainview, some mods use 0 or 1 for x
	if ((refdef->x == 0) || (refdef->x == 1)) {
		// Copy main refdef
		memcpy(&nex.cg_refdef, refdef, sizeof(refdef_t));
		vectoangles(nex.cg_refdef.viewaxis[0], nex.cg_refdefViewAngles);

		if (nexValue[VAR_NOWEAPONZOOM]) {
			switch (nex.cg_snap->ps.weapon) {
				case WP_FG42SCOPE:
				case WP_GARAND_SCOPE:
				case WP_K43_SCOPE:
				case WP_MOBILE_MG42_SET:
					// fov_x - Get from cg_fov cvar
					nexSyscall.Cvar_VariableStringBuffer("cg_fov", fov, sizeof(fov));
					refdef->fov_x = atof(fov);

					// fov_y - Algo taken from cg_view.c line 961
					x = refdef->width / tan(refdef->fov_x / 360 * M_PI);
					refdef->fov_y = atan2(refdef->height, x);
					refdef->fov_y *= 360 / M_PI;
					break;
				default:
					break;
			}
		}

		// Call original function
		orig_syscall(CG_R_RENDERSCENE, refdef);

		if ((nex.cg_snap->ps.stats[STAT_HEALTH] > 0) && (nex.cgs_gamestate != GS_INTERMISSION)) {
			if (nexValue[VAR_MIRRORCAM])
				nexSpycam.mirror();
			if ((nex.cg_snap->ps.weapon == WP_MORTAR_SET) && nexValue[VAR_MORTARCAM])
				nexSpycam.mortar();
			else if ((nex.cg_snap->ps.weapon == WP_GPG40 || nex.cg_snap->ps.weapon == WP_M7) && nexValue[VAR_RIFLECAM])
				nexSpycam.rifle();
			else if ((nex.cg_snap->ps.weapon == WP_SATCHEL_DET) && nexValue[VAR_SATCHELCAM])
				nexSpycam.satchel();
			else if ((nex.cg_snap->ps.weapon == WP_PANZERFAUST) && nexValue[VAR_PANZERCAM])
				nexSpycam.panzer();
		}
		return false;
	}
	
	return true;
}

bool CNexEngine::CG_R_DrawStretchPic(float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader) {
	nexDraw.adjustTo640(&x, &y, &w, &h);

	// Remove sniper/binocular picture
	if (hShader == nex.media.reticleShaderSimple || hShader == nex.media.binocShaderSimple)
		return false;
	// Remove sniper/binocular black area
	if (((int)y == 0) && ((int)w == 80))
		return false;

	return true;
}

bool CNexEngine::isVisible(vec3_t target) {
	trace_t trace;
	nex.orig.CG_Trace(&trace, nex.cg_refdef.vieworg, NULL, NULL, target, nex.cg_snap->ps.clientNum, CONTENTS_SOLID | CONTENTS_CORPSE);
	return (trace.fraction == 1.0f);
}

bool CNexEngine::isPlayerVisible(vec3_t target, int player) {
	trace_t traceVisible;
	nex.orig.CG_Trace(&traceVisible, nex.cg_refdef.vieworg, NULL, NULL, target, nex.cg_snap->ps.clientNum, CONTENTS_SOLID | CONTENTS_CORPSE);

	trace_t tracePlayer;
	nex.orig.CG_Trace(&tracePlayer, nex.cg_refdef.vieworg, NULL, NULL, target, nex.cg_snap->ps.clientNum, CONTENTS_SOLID | CONTENTS_BODY);

	return ((traceVisible.fraction == 1.0f) && (tracePlayer.entityNum == player));
}

void CNexEngine::setMuzzle() {
	vec3_t forward, right, up;

	VectorCopy(nex.cg_snap->ps.origin, nex.muzzle);
	nex.muzzle[2] += nex.cg_snap->ps.viewheight;
	AngleVectors(nex.cg_snap->ps.viewangles, forward, right, up);

	switch (nex.cg_snap->ps.weapon)	{
		case WP_PANZERFAUST:
			VectorMA(nex.muzzle, 10, right, nex.muzzle);
			break;
		case WP_DYNAMITE:
		case WP_GRENADE_PINEAPPLE:
		case WP_GRENADE_LAUNCHER:
		case WP_SATCHEL:
		case WP_SMOKE_BOMB:
			VectorMA(nex.muzzle, 20, right, nex.muzzle);
			break;
		case WP_AKIMBO_COLT:
		case WP_AKIMBO_SILENCEDCOLT:
		case WP_AKIMBO_LUGER:
		case WP_AKIMBO_SILENCEDLUGER:
			VectorMA(nex.muzzle, -6, right, nex.muzzle);
			VectorMA(nex.muzzle, -4, up, nex.muzzle);
			break;
		default:
			VectorMA(nex.muzzle, 6, right, nex.muzzle);
			VectorMA(nex.muzzle, -4, up, nex.muzzle);
			break;
	}
	SnapVector(nex.muzzle);
}

int CNexEngine::findSatchel() {
	int entityNum = 0;
	for (; entityNum < MAX_GENTITIES; entityNum++) {
		if ((nex.cg_entities[entityNum].currentState->weapon == WP_SATCHEL)
				&& (nex.cg_entities[entityNum].currentState->clientNum == nex.cg_snap->ps.clientNum)
				&& (!VectorCompare(nex.entities[entityNum].origin, vec3_origin)))
			return entityNum;
	}

	return -1;
}
