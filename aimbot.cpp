// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

CNexAimbot nexAimbot;

// this one is used for bind-style aimkey. usage: bind MOUSE1 "+aimbot"
void aimbotUpCommand (void) {}

// This tags are in priority order. The first tags are near the center of a
// model, the last tag is at the edge of the model
static char *modelBodyTags[] = {
	"tag_chest",
	"tag_torso",
	"tag_bright",
	"tag_lbelt",
	"tag_bleft",
	"tag_ubelt",
	"tag_back",
	"tag_armleft",
	"tag_weapon",
	"tag_armright",
	"tag_weapon2",
	"tag_legright",
	"tag_legleft",
	"tag_footleft",
	"tag_footright"
};

bool CNexAimbot::getVisibleModelBodyTag(const int player, vec3_t *origin) {
	int count = 0;
	for (; count < (sizeof(modelBodyTags) / sizeof(char *)); count++) {
		char *tagText = modelBodyTags[count];
		orientation_t tagOrientation;

		if (!CG_GetTag(player, tagText, &tagOrientation)) {
			#ifdef NEX_LOGGING
				nexUtils.log("ERROR [getVisibleModelBodyTag]: can't find tag %i [%s] for player: %i", count, tagText, player);
			#endif
			return false;
		}

		if (nexEngine.isVisible(tagOrientation.origin)) {
			VectorCopy(tagOrientation.origin, *origin);
			return true;
		}
		
	}
	return false;
}

bool CNexAimbot::CG_GetTag(int clientNum, char *tagname, orientation_t *or) {
	nex_centity_t	*cent;
	refEntity_t		*refent;
	vec3_t			tempAxis[3];
	vec3_t			org;
	int				i;

	cent = &nex.cg_entities[clientNum];

	if (!cent->currentValid)
		return false;		// not currently in PVS

	refent = &nex.cg_entities[clientNum].pe_bodyRefEnt;

	if (nexSyscall.R_LerpTag(or, refent, tagname, 0 ) < 0)
		return false;

	VectorCopy(nex.entities[clientNum].origin, org);

	for (i = 0 ; i < 3 ; i++)
		VectorMA(org, or->origin[i], refent->axis[i], org);

	VectorCopy(org, or->origin);

	// rotate with entity
	MatrixMultiply(refent->axis, or->axis, tempAxis);
	memcpy(or->axis, tempAxis, sizeof(vec3_t) * 3);

	return true;
}

void CNexAimbot::doAutoShoot(bool shoot) {
	// Don't shoot if we're specing/following a player
	if (nex.cg_clientNum != nex.cg_snap->ps.clientNum)
		return;

	if (shoot) {
		// Check for sniper weapon delay
		if (((nex.cg_entities[nex.cg_clientNum].currentState->weapon == WP_K43_SCOPE) || (nex.cg_entities[nex.cg_clientNum].currentState->weapon == WP_GARAND_SCOPE)) && !BG_PlayerMounted(nex.cg_snap->ps.eFlags)) {
			static int lastShootTime = 0;
			if ((nex.cg_time - lastShootTime) >= nexValue[VAR_SNIPERDELAY]) {
				nexSyscall.SendConsoleCommand("+attack\n");
				lastShootTime = nex.cg_time;
			} else {
				nexSyscall.SendConsoleCommand("-attack\n");
			}
		// Check for weapon overheating
		#define OVERHEATING_LIMIT 200
		} else if (nexValue[VAR_OVERHEAT] && (nex.cg_snap->ps.curWeapHeat > OVERHEATING_LIMIT)) {
			nexSyscall.SendConsoleCommand("-attack\n");
		} else {
			nexSyscall.SendConsoleCommand("+attack\n");
		}
	} else {
		nexSyscall.SendConsoleCommand("-attack\n");
	}
}

void CNexAimbot::doAimbot() {
	bool autoShootWeapon = false;
	int targetFlag[2] = { -1, -1 };	// If a targetFlag failed try the next one

	// if player is mounted (tank mg, mobile mg42, static mg42) aim for body
	if (BG_PlayerMounted(nex.cg_snap->ps.eFlags) || (nex.cg_snap->ps.weapon == WP_MOBILE_MG42) || (nex.cg_snap->ps.weapon == WP_MOBILE_MG42_SET)) {
		autoShootWeapon = true;
		targetFlag[0] = TARGET_PLAYER_ENEMY | TARGET_PLAYER_BODY;

		targetFlag[1] = targetFlag[0] | TARGET_PLAYER_HEAD;
	} else {
		// different targets for different weapons
		switch (nex.cg_snap->ps.weapon) {
			case WP_LUGER:
			case WP_SILENCER:
			case WP_AKIMBO_LUGER:
			case WP_AKIMBO_SILENCEDLUGER:
			case WP_COLT:
			case WP_SILENCED_COLT:
			case WP_AKIMBO_COLT:
			case WP_AKIMBO_SILENCEDCOLT:
			case WP_MP40:
			case WP_THOMPSON:
			case WP_GARAND:
			case WP_K43:
			case WP_FG42:
			case WP_STEN:
			case WP_CARBINE:
			case WP_KAR98:
				autoShootWeapon = true;
				targetFlag[0] = TARGET_PLAYER_ENEMY;
		
				// Aim priority
				if (nexValue[VAR_HEADONLY])
					targetFlag[0] |= TARGET_PLAYER_HEAD;
				else
					targetFlag[0] |= TARGET_PLAYER_HEAD_BODY;
				break;
			case WP_GPG40:
			case WP_M7:
				autoShootWeapon = true;
				targetFlag[0] = TARGET_PLAYER_ENEMY | TARGET_PLAYER_ALIVE | TARGET_PLAYER_BODY;
				break;
			case WP_GARAND_SCOPE:
			case WP_K43_SCOPE:
			case WP_FG42SCOPE:
				autoShootWeapon = true;
				targetFlag[0] = TARGET_PLAYER_ENEMY | TARGET_PLAYER_ALIVE | TARGET_PLAYER_HEAD;
				break;
			case WP_KNIFE:
				targetFlag[0] = TARGET_PLAYER_ENEMY | TARGET_PLAYER_ALIVE | TARGET_PLAYER_HEAD;
				targetFlag[1] = TARGET_PLAYER_ENEMY | TARGET_PLAYER_DEAD | TARGET_PLAYER_HEAD;
				break;
			default:
				break;
		}
	}
	targetFlag[0] |= TARGET_PLAYER_ALIVE;
	// Check if we want aimbot
	if ((nexSyscall.isKeyActionDown(sNex.aimKey) || nexValue[VAR_AUTO])
			&& (targetFlag[0] != -1) && (nex.cg_snap->ps.clientNum == nex.cg_clientNum)) {
		// Find target aim point
		int count = 0;
		int target;
		for (; count < (sizeof(targetFlag) / sizeof(int)); count++) {
			if (targetFlag[count] != -1) {
				vec3_t aimPoint;
				VectorCopy(vec3_origin, aimPoint);

				target = findNearestTarget(targetFlag[count]);

				// No target found
				if (target == -1)
					continue;
			
				// Target found
				if (targetFlag[count] & TARGET_PLAYER) {
					if (targetFlag[count] & TARGET_PLAYER_HEAD) {
						VectorCopy(nex.entities[target].head, aimPoint);
					} else if (targetFlag[count] & TARGET_PLAYER_BODY) {
						VectorCopy(nex.entities[target].bodyPart, aimPoint);
					} else if (targetFlag[count] & TARGET_PLAYER_HEAD_BODY) {
						// Head or body ?
						if (nex.entities[target].isHeadVisible)
							VectorCopy(nex.entities[target].head, aimPoint);
						else
							VectorCopy(nex.entities[target].bodyPart, aimPoint);
					} else {
						#ifdef NEX_LOGGING
												nexUtils.log("ERROR AIMBOT: target found but no aimpoint set in target flags");
						#endif
					}
				} else if (targetFlag[count] & TARGET_MISSILE) {
					VectorCopy(nex.entities[target].origin, aimPoint);
				} else {
					#ifdef NEX_LOGGING
						nexUtils.log("ERROR AIMBOT: target found but it's not a player/missile");
					#endif
				}
				
				// If aim point is set, aim and shoot
				if (!VectorCompare(aimPoint, vec3_origin)) {
					if (!nexValue[VAR_HUMAN])
					    doAutoShoot(true);

					if ((nex.cg_snap->ps.weapon == WP_GPG40) || (nex.cg_snap->ps.weapon == WP_M7))
						rifleAimAt(aimPoint);
					else
						aimAt(aimPoint);

					return;
				}
			}
		}
	}
	doAutoShoot(nexSyscall.isKeyActionDown("+attack"));
}

int CNexAimbot::findNearestTarget(int targetFlag) {
	int nearest = -1;
	int entityNum = 0;

	for (; entityNum < MAX_CLIENTS; entityNum++) {
		nexEntity_t* entity = &nex.entities[entityNum];

		if (!nex.cg_entities[entityNum].currentValid || (entityNum == nex.cg_snap->ps.clientNum) || !nex.client[entityNum].infoValid)
			continue;

		// Player
		if ((targetFlag & TARGET_PLAYER) && !IS_PLAYER_VALID(entityNum))
			continue;
		if ((targetFlag & TARGET_PLAYER_VULN) && !IS_PLAYER_VULNERABLE(entityNum))
			continue;
		if ((targetFlag & TARGET_PLAYER_ALIVE) && IS_PLAYER_DEAD(entityNum))
			continue;
		if ((targetFlag & TARGET_PLAYER_DEAD) && !IS_PLAYER_DEAD(entityNum))
			continue;

		// Target
		if ((targetFlag & TARGET_PLAYER_HEAD) && !entity->isHeadVisible)
			continue;
		if ((targetFlag & TARGET_PLAYER_BODY) && VectorCompare(entity->bodyPart, vec3_origin))
			continue;

		// Common
		if ((targetFlag & TARGET_ENEMY) && !IS_PLAYER_ENEMY(entityNum))
			continue;
		if ((targetFlag & TARGET_FRIEND) && IS_PLAYER_ENEMY(entityNum))
			continue;

		if ((targetFlag & TARGET_VISIBLE) && !entity->isVisible)
			continue;
		if ((targetFlag & TARGET_NOTVISIBLE) && entity->isVisible)
			continue;

		float aimfov = AngleNormalize360(nexValue[VAR_AIMFOV]);
		if (aimfov) {
			vec3_t aimPoint, ang, org;
			if (nex.entities[entityNum].isHeadVisible)
				VectorCopy(nex.entities[entityNum].head, aimPoint);
			else if (!nexValue[VAR_HEADONLY])
				VectorCopy(nex.entities[entityNum].bodyPart, aimPoint);
			else {
				#ifdef NEX_LOGGING
					nexUtils.log("ERROR AIMBOT: findNearestTarget, can't find a point to aim");
				#endif
				return -1;
			}

			VectorSubtract(aimPoint, nex.cg_refdef.vieworg, org);
			vectoangles(org, ang);

			ang[PITCH] -= nex.cg_refdefViewAngles[PITCH];
			ang[YAW] -= nex.cg_refdefViewAngles[YAW];


			if (ang[PITCH] < 0.0)
				ang[PITCH] *= -1.0;
			if (ang[YAW] < 0.0)
				ang[YAW] *= -1.0;

			if (ang[PITCH] > 360.0f)
				ang[PITCH] -= 360.0f;
			if (ang[YAW] > 360.0f)
				ang[YAW] -= 360.0f;

			float aimFov[2];

			aimFov[0] = (ang[PITCH] - 360.0) * -1.0;
			aimFov[1] = (ang[YAW] - 360.0) * -1.0;

			if (ang[PITCH] > aimFov[0])
				ang[PITCH] = aimFov[0];
			if (ang[YAW] > aimFov[1])
				ang[YAW] = aimFov[1];

			if ((ang[PITCH] > aimfov) || (ang[YAW] > aimfov))
				continue;
		}

		if ((entity->distance != 0)
				&& (nearest == -1 || (entity->distance < nex.entities[nearest].distance))) {
			nearest = entityNum;
		}
	}

	return nearest;
}

void CNexAimbot::aimAt(vec3_t target) {
	if (nexValue[VAR_AIMPOINT]) {
		#define POINT_SIZE 2
    	int x, y;
		if (nexDraw.worldToScreen(target, &x, &y))
			nexDraw.drawFillRect(x - (POINT_SIZE / 2), y - (POINT_SIZE / 2), POINT_SIZE, POINT_SIZE, colorYellow);
	}

	vec3_t org, ang;

	// Self Prediction
	if (nexValue[VAR_SELFPREDICT]) {
		vec3_t pVieworg;
		VectorMA(nex.cg_refdef.vieworg, (float)(nex.cg_frametime) / 1000.0f, nex.cg_entities[nex.cg_clientNum].currentState->pos.trDelta, pVieworg);
		VectorSubtract(target, pVieworg, org);
    } else {
		VectorSubtract(target, nex.cg_refdef.vieworg, org);
	}

	vectoangles(org, ang);

	ang[PITCH] -= nex.cg_refdefViewAngles[PITCH];
	ang[YAW] -= nex.cg_refdefViewAngles[YAW];																											


	// Simple human aim for now
	if (nexValue[VAR_HUMAN]) {
		while (ang[PITCH] < -360.0f)
			ang[PITCH] += 360.0f;
		while (ang[PITCH] > 360.0f)
			ang[PITCH] -= 360.0f;
		while (ang[YAW] < -360.0f)
			ang[YAW] += 360.0f;
		while (ang[YAW] > 360.0f)
			ang[YAW] -= 360.0f;
		if (ang[PITCH] > 180.0f)
			ang[PITCH] -= 360.0f;
		if (ang[PITCH] < -180.0f)
			ang[PITCH] += 360.0f;
		if (ang[YAW] > 180.0f)
			ang[YAW] -= 360.0f;
		if (ang[YAW] < -180.0f)
			ang[YAW] += 360.0f;
		ang[PITCH] *= nexValue[VAR_HUMANVALUE];
		ang[YAW] *= nexValue[VAR_HUMANVALUE];
	}

    *(float *)sNex.ET->cl_mouseDy += ang[PITCH];
    *(float *)sNex.ET->cl_mouseDx += ang[YAW];
}

void CNexAimbot::rifleAimAt(vec3_t target) {
	vec3_t org, ang;

	VectorSubtract(target, nex.cg_refdef.vieworg, org);
	vectoangles(org, ang);


	// Calculate y distance
	vec3_t me, enemy;
	VectorCopy(nex.cg_refdef.vieworg, me);
	VectorCopy(target, enemy);
	me[ROLL] = 0;
	enemy[ROLL] = 0;
	float y = target[ROLL] - nex.cg_refdef.vieworg[ROLL];

	float v = 2000;	// Riffle speed
	float g = DEFAULT_GRAVITY;
	float x = VectorDistance(me, enemy);
	
	float angle = atan((pow(v, 2) - (sqrt(pow(v, 4) + (g * ((g * pow(x, 2)) + (2.0f * y * pow(v, 2))))))) / (g * x));
	angle = RAD2DEG(angle);
	ang[PITCH] = angle;


	ang[PITCH] -= nex.cg_refdefViewAngles[PITCH];
	ang[YAW] -= nex.cg_refdefViewAngles[YAW];

    *(float *)sNex.ET->cl_mouseDy += ang[PITCH];
    *(float *)sNex.ET->cl_mouseDx += ang[YAW];
}

void CNexAimbot::doVecZ(int player) {
	nexEntity_t *entity = &nex.entities[player];

	// No need if not an enemy
	if (!IS_PLAYER_ENEMY(player))
		return;

	// Set different vecz for different weapon
	switch (nex.cg_snap->ps.weapon) {
		case WP_LUGER:
		case WP_SILENCER:
		case WP_AKIMBO_LUGER:
		case WP_AKIMBO_SILENCEDLUGER:
		case WP_COLT:
		case WP_SILENCED_COLT:
		case WP_AKIMBO_COLT:
		case WP_AKIMBO_SILENCEDCOLT:
		case WP_MP40:
		case WP_THOMPSON:
	 	case WP_MOBILE_MG42:
		case WP_GARAND:
		case WP_K43:
		case WP_FG42:
		case WP_STEN:
		case WP_CARBINE:
		case WP_KAR98:
		case WP_GARAND_SCOPE:
		case WP_K43_SCOPE:
		case WP_FG42SCOPE:
			// Auto vec Z correction
			if (nexValue[VAR_AUTOVECZ]) {
				#define NEAR_TARGET_VECZ 5.9f

				// Scoped weapons always aim at the middle of head
				if (nex.cg_snap->ps.weapon == WP_GARAND_SCOPE || nex.cg_snap->ps.weapon == WP_K43_SCOPE || nex.cg_snap->ps.weapon == WP_FG42SCOPE) {
					VectorMA(entity->head, NEAR_TARGET_VECZ, entity->headAxis[2], entity->head);
					break;
				}

				// Different 'near distance' for sten
				int nearTarget;
				if (nex.cg_snap->ps.weapon == WP_STEN)
					nearTarget = 700.0f;
				else
					nearTarget = 500.0f;

				if (entity->distance <= nearTarget)
					VectorMA(entity->head, NEAR_TARGET_VECZ, entity->headAxis[2], entity->head);
				else if ((entity->distance > nearTarget) && (entity->distance < 2000.0f))
					VectorMA(entity->head, (NEAR_TARGET_VECZ * (100.0f - ((entity->distance - nearTarget) / 15.0f))) / 100.0f, entity->headAxis[2], entity->head);
			} else { // Manual vec Z correction
					switch (nex.cg_snap->ps.weapon) {
						case WP_STEN:
							VectorMA(entity->head, nexValue[VAR_VECZ_STEN], entity->headAxis[2], entity->head);
							break;
						case WP_K43_SCOPE:
						case WP_GARAND_SCOPE:
						case WP_FG42SCOPE:
							VectorMA(entity->head, nexValue[VAR_VECZ_SCOPE], entity->headAxis[2], entity->head);
							break;
						default:
							VectorMA(entity->head, nexValue[VAR_VECZ], entity->headAxis[2], entity->head);
							break;
					}
			}
			break;
		default:
			break;
	}
}
