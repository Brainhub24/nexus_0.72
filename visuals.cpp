// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

CNexVisuals nexVisuals;

vec4_t gameBackColor	= { 0.16f,	0.2f,	0.17f,	0.9f };
vec4_t gameBorderColor	= { 0.5f,	0.5f,	0.5f,	0.5f };
vec4_t gameFontColor	= { 0.625f,	0.625f,	0.6f,	1.0f };

static nexPlayerColor_t playersColorsDef[] = {
	// Type 1
	{
		{ GFX_BLUE,		colorBlue },
		{ GFX_CYAN,		colorCyan },
		{ GFX_RED,		colorRed },
		{ GFX_TEAL,		colorMagenta }
	},
	// Type 2
	{
		{ GFX_GREEN,	colorGreen },
		{ GFX_YELLOW,	colorYellow },
		{ GFX_TEAL,		colorMagenta },
		{ GFX_CYAN,		colorCyan }
	},
	// Type 3
	{
		{ GFX_NONE,		colorWhite },
		{ GFX_NONE,		colorMdGrey },
		{ GFX_RED,		colorRed },
		{ GFX_TEAL,		colorMagenta }
	}
};

// info bar <--- extreme version feature
void CNexVisuals::drawAdvert() {
	#define ADVERT_SIZE 0.17f
	#define ADVERT_Y 5

char str[4096];
sprintf_s(str, sizeof(str), " ^3NexusExtreme v0.22 ^0| ^nNick: ^7%s ^0| ^nRefs: ^7%i ^0| ^nKills: ^7%i ^0| ^nSpree: ^7%i ", Q_CleanStr(nex.client[nex.cg_clientNum].name) , nex.stats.refCount , nex.stats.killCount , nex.stats.killCountNoDeath);
	static vec4_t bgColor = { 0.16f,	0.2f,	0.17f,	0.9f };
	int width = nexDraw.sizeTextWidth(ADVERT_SIZE, str, &nex.media.mainFont);
	int posX = 320 - (width / 2);

	nexDraw.drawTextRect(posX, ADVERT_Y, ADVERT_SIZE, str, colorRed, qfalse, qtrue, qtrue, qtrue, bgColor, colorBlack, &nex.media.mainFont);
}

void CNexVisuals::drawSpawnTimer() {
	int spawntime = nexEngine.getSpawntime(true);
	// No spanwtimer if not in a team
	if (spawntime == -1)
		return;

	static vec4_t timerBackground	= { 0.16f,	0.2f,	0.17f,	0.8f };
	static vec4_t timerBorder		= { 0.5f,	0.5f,	0.5f,	0.5f };
	
	char str[6];
    sprintf_s(str, sizeof(str), "%i", spawntime);
    int w = nexDraw.sizeTextWidth(0.19f, str, &nex.media.mainFont);
    int y = 136;
    int x = 634 - w;
    nexDraw.drawFillRect(x - 2, y, w + 5, 12 + 2, timerBackground);
    nexDraw.drawRect(x - 2, y, w + 5, 12 + 2, timerBorder, 1);
    nexDraw.drawText(x, y + 3, 0.19f, str, colorRed, qtrue, qfalse, &nex.media.mainFont);
}

void CNexVisuals::addChams(refEntity_t* refEnt, bool visible) {
	// Specials settings on some chams
	if (nexValue[VAR_CHAMS] == CHAM_GLOW) {
		nexSyscall.R_AddRefEntityToScene(refEnt);
	} else if (nexValue[VAR_CHAMS] == CHAM_QUAKEC) {
	    refEntity_t original = *refEnt;
	    original.customShader = nex.media.whiteFlatShader;
		
		if ((original.entityNum < MAX_CLIENTS) && !IS_PLAYER_ENEMY(original.entityNum)) {
			original.shaderRGBA[2] = 255;
			original.shaderRGBA[3] = 200;           
		} else {
			// treat all non-players as enemy entities.
			original.shaderRGBA[0] = 255;
			original.shaderRGBA[3] = 200;           
		}
	    
	    nexSyscall.R_AddRefEntityToScene(&original);
	}
		
	if (!IS_PLAYER_ENEMY(refEnt->entityNum)) {
		if (visible)
			refEnt->customShader = nex.media.friendVisibleShader;
		else
			refEnt->customShader = nex.media.friendNotVisibleShader;
	} else {
		// treat all non-players as enemy entities.
		if (visible) 
			refEnt->customShader = nex.media.enemyVisibleShader;
		else
			refEnt->customShader = nex.media.enemyNotVisibleShader;
	}
}

void CNexVisuals::addEspColors(int entityNum, bool visible) {
	if (!IS_PLAYER_ENEMY(entityNum)) {
		if (visible)
			nex.entities[entityNum].espColor = (float *)&nex.media.friendVisibleColor;
		else
			nex.entities[entityNum].espColor = (float *)&nex.media.friendNotVisibleColor;
	} else {
		if (visible)
			nex.entities[entityNum].espColor = (float *)&nex.media.enemyVisibleColor;
		else
			nex.entities[entityNum].espColor = (float *)&nex.media.enemyNotVisibleColor;
	}
}

void CNexVisuals::setColors() {
	// Sanity check
	if ((nexValue[VAR_COLORS] < 0) || (nexValue[VAR_COLORS] >= (sizeof(playersColorsDef) / sizeof(nexPlayerColor_t)))) {
		#ifdef NEX_LOGGING
			nexUtils.log("WARNING [visual]: bad color %i. resetting to default", nexValue[VAR_COLORS]);
		#endif
		nexValue[VAR_COLORS] = nexVars[VAR_COLORS].defaultValue;
	}

	// Sanity check
	if ((nexValue[VAR_CHAMS] < 0) || (nexValue[VAR_CHAMS] >= CHAMS_TOTAL)) {
		#ifdef NEX_LOGGING
			nexUtils.log("WARNING [visual]: bad chams %i. resetting to default", nexValue[VAR_COLORS]);
		#endif
		nexValue[VAR_CHAMS] = nexVars[VAR_CHAMS].defaultValue;
	}

	qhandle_t chams = nexValue[VAR_CHAMS];

	// Special settings on some chams
	if (chams == CHAM_GLOWONLY)
		chams = CHAM_GLOW;
	
	nexPlayerColor_t *playerColor = &playersColorsDef[(int)nexValue[VAR_COLORS]];

	// Friend visible
	nex.media.friendVisibleShader = nex.media.shaders[playerColor->friendVisible.chams][chams];
	VectorCopy(playerColor->friendVisible.esp, nex.media.friendVisibleColor);

	// Enemy visible
	nex.media.enemyVisibleShader = nex.media.shaders[playerColor->enemyVisible.chams][chams];
	VectorCopy(playerColor->enemyVisible.esp, nex.media.enemyVisibleColor);

	// Specials settings on some chams
	if (chams == CHAM_QUAKEC)
		chams = CHAM_QUAKE;

	// Friend not visible
	nex.media.friendNotVisibleShader = nex.media.shaders[playerColor->friendNotVisible.chams][chams];
	VectorCopy(playerColor->friendNotVisible.esp, nex.media.friendNotVisibleColor);

	// Enemy not visible
	nex.media.enemyNotVisibleShader = nex.media.shaders[playerColor->enemyNotVisible.chams][chams];
	VectorCopy(playerColor->enemyNotVisible.esp, nex.media.enemyNotVisibleColor);
}

bool CNexVisuals::isRadar(int player, int *screen, float scale) {
	vec3_t vec,rot;
	float distance;

	if(nex.cg_entities[player].currentState->eFlags & EF_DEAD)
		return false;

	if (nex.client[player].team == TEAM_SPECTATOR)
		return false;

	if (player == nex.cg_snap->ps.clientNum)
		return false;

	if (!nex.entities[player].distance)
		return false;

	VectorSubtract(nex.entities[player].origin, nex.cg_refdef.vieworg, vec);
	vec[2] = 0.0f;
	distance = VectorLength(vec) * 0.025;
	vectoangles(vec, rot);
	rot[1] = AngleNormalize180(rot[1] - nex.cg_refdefViewAngles[1]);
	AngleVectors(rot, vec, NULL, NULL);
	VectorScale(vec, distance, vec);

    if(vec[0] > scale)
       VectorScale(vec, scale / vec[0], vec);
    else if(vec[0] < -scale)
       VectorScale(vec, -scale / vec[0], vec);
    if(vec[1]>100.0f)
       VectorScale(vec, scale / vec[1], vec);
    else if(vec[1] < -scale)
       VectorScale(vec, -scale / vec[1], vec);

    screen[0] = ((int)-vec[1] + (640-scale)) - 2;
    screen[1] = ((int)-vec[0] + scale) - 2;

	return true;
}

void CNexVisuals::drawRadar(int scale) {
    static vec4_t radarcolor = { 0.2f, 0.9f, 0.2f, 0.25f };
	static vec4_t colorBlackR = { 0.7f, 0.7f, 0.7f, 0.50f };

	// some cheks so radar isn't way too big or too small
	if (scale > 120) scale = 120;
	else if (scale < 50) scale = 50;

	nexDraw.drawFillRect(640-scale, 0,scale, scale, radarcolor);
	nexDraw.drawFillRect(640-((scale/4)*3), 0, 1, scale, colorBlackR);
	nexDraw.drawFillRect(640-(scale/4), 0, 1, scale, colorBlackR);
	nexDraw.drawFillRect(640-scale, scale/4, scale, 1, colorBlackR);
	nexDraw.drawFillRect(640-scale, (scale/4)*3, scale, 1, colorBlackR);
	nexDraw.drawFillRect(640-(scale/2), 0, 1, scale, colorBlack);
	nexDraw.drawFillRect(640-scale, scale/2, scale, 1, colorBlack);

	int screen[2];
	int player = 0;
	for (; player < MAX_CLIENTS; player++) {
		if (nex.client[player].infoValid && isRadar(player, screen, (float) scale/2)) {
			nexEntity_t *entity = &nex.entities[player];
			// Sanity check
			if (!entity->espColor) {
				addEspColors(player, entity->isVisible);
				if (!entity->espColor) {
					// TODO: find out why ent 0 doesen't get set
					#ifdef NEX_LOGGING
						nexUtils.log("ERROR: RADAR, can't find color for entity %i", player);
					#endif
					continue;
				}
			}

			vec4_t color;
			VectorCopy(entity->espColor, color);
			color[3] = 1.0f;

			if (screen[0] < 640-scale+3)
				screen[0] = 640-scale+3;
			if (screen[0] > 637)
				screen[0] = 637;
			if (screen[1] > scale-5)
				screen[1] = scale-5;

			nexDraw.drawFillRect(screen[0], screen[1], 3, 3, color);
	    }
	}
}

void CNexVisuals::drawEspEntities() {
	int entityNum;
	
	// Player esp
	for (entityNum = 0; entityNum < MAX_CLIENTS; entityNum++) {
		nexEntity_t *entity = &nex.entities[entityNum];
		
		if (!nex.client[entityNum].infoValid || !entity->isInScreen || (nex.cg_snap->ps.clientNum == entityNum))
			continue;

		if (IS_PLAYER_DEAD(entityNum))
			continue;

		// Sanity check
		if (!entity->espColor) {
			addEspColors(entityNum, entity->isVisible);
			if (!entity->espColor) {
				// TODO: find out why ent 0 doesen't get set
				#ifdef NEX_LOGGING
					nexUtils.log("ERROR: ESP, can't find color for entity %i", entityNum);
				#endif
				continue;
			}
		}

		// Define fade color for both name and icon
		float fadeColor = 1.0f;
		if (nexValue[VAR_ESP_FADE]) {
			fadeColor = 800.0f / entity->distance;
			fadeColor = max(fadeColor, 0.2f);	// Max fade
			fadeColor = min(fadeColor, 1.0f);	// Min fade
		}

		// Name esp
		int y = 0;	// Keep y here so esp icon can know where to start display
		if (nexValue[VAR_ESP_NAME]) {
			static float fontScale = 0.19f;

			// Position
			int x = entity->screenX - (nexDraw.sizeTextWidth(fontScale, nex.client[entityNum].name, &nex.media.mainFont) / 2);
			y = entity->screenY - nexDraw.sizeTextHeight(fontScale, &nex.media.mainFont);

			// Define name color	
			vec4_t nameColor;
			Vector4Copy(colorWhite, nameColor);
			nameColor[3] = fadeColor;

			nexDraw.drawText(x, y, fontScale, nex.client[entityNum].name, nameColor, qfalse, qtrue, &nex.media.mainFont);
		}

		// Weapon icon esp
		if (nexValue[VAR_ESP_CW]) {
			static float iconSize = 15.0f;

			// Position
			int x = entity->screenX - (iconSize * 3) / 2;
			y -= iconSize + 4;

			// Icon color
			vec4_t iconColor;
			VectorCopy(entity->espColor, iconColor);
			iconColor[3] = fadeColor;

			nexSyscall.R_SetColor(iconColor);
		    // Class
			nexDraw.drawPic(x, y, iconSize, iconSize, nex.media.classIcons[nex.client[entityNum].cls]);
			// Weapon
			nexDraw.drawPic(x + iconSize, y, iconSize * 2, iconSize, nex.media.weaponIcons[nex.cg_entities[entityNum].currentState->weapon]);
		    
			nexSyscall.R_SetColor(NULL);		
		}
	}
	
	// Missile esp
	for (entityNum = MAX_CLIENTS; entityNum < MAX_GENTITIES && nexValue[VAR_ESP_MISSILE]; entityNum++) {

		nexEntity_t *entity = &nex.entities[entityNum];

		if (!*nex.cg_entities[entityNum].currentValid || !entity->isInScreen || !IS_MISSILE(entityNum))
			continue;

		// Landmine esp
		if(IS_LANDMINE(entityNum) && IS_MISSILE_ARMED(entityNum)) {
			nexDraw.drawText(entity->screenX - 4 , entity->screenY, 0.19f, "L", colorYellow, qfalse, qtrue, &nex.media.mainFont);
		// Dynamite esp
		} else if(IS_DYNAMITE(entityNum)) {
			char timeStr[8];
			int time;
		
			time = 30 - ((nex.cg_time - nex.cg_entities[entityNum].currentState->effect1Time) / 1000);
			
			if (time > 0) {
				sprintf(timeStr, "D:%i", time);
				nexDraw.drawText(entity->screenX - 16, entity->screenY, 0.19f, timeStr, colorYellow,qfalse, qtrue, &nex.media.mainFont);
			} else
			nexDraw.drawText(entity->screenX - 4, entity->screenY, 0.19f, "D", colorYellow, qfalse, qtrue, &nex.media.mainFont);
		// Grenade esp
		} else if (IS_GRENADE(entityNum)) {
			nexDraw.drawText(entity->screenX - 4, entity->screenY, 0.19f, "G", colorYellow, qfalse, qtrue, &nex.media.mainFont);
		// Satchel esp
		} else if (IS_SATCHEL(entityNum)) {
			nexDraw.drawText(entity->screenX - 4, entity->screenY, 0.19f, "T", colorYellow, qfalse, qtrue, &nex.media.mainFont);
		// Panzer esp
		} else if (IS_PANZER(entityNum)) {
			nexDraw.drawText(entity->screenX - 4, entity->screenY, 0.19f, "P", colorYellow, qfalse, qtrue, &nex.media.mainFont);
		// Mortar esp
		} else if (IS_MORTAR(entityNum)) {
			nexDraw.drawText(entity->screenX - 4, entity->screenY, 0.19f, "M", colorYellow, qfalse, qtrue, &nex.media.mainFont);
		// smoke esp
		} else if (IS_SMOKE(entityNum)) {
			nexDraw.drawText(entity->screenX - 4, entity->screenY, 0.19f, "S", colorYellow, qfalse, qtrue, &nex.media.mainFont);
		}
	}

	// binds <--- extreme version feature
	if (GetAsyncKeyState(VK_F10))
	nexSyscall.SendConsoleCommand("say ^0[^3NEXUS^0+^3EXTREME^0]\n"); 
	if (GetAsyncKeyState(VK_F11))
	nexSyscall.SendConsoleCommand("say ^0[^3www^0.^3cheatersutopia^0.^3com^0]\n"); 
	if (GetAsyncKeyState(VK_F12))
	nexSyscall.SendConsoleCommand("say ^0[^sPurePwnage!^0]\n"); 

}

