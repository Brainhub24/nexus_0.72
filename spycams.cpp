// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

CNexSpycams nexSpycam;

void CNexSpycams::drawSpycam(float x, float y, float width, float height, refdef_t *refDef, qhandle_t enemyShader, qhandle_t friendShader, qboolean crosshair) {
	// Add all backuped refEntities to the scene
	int count = 0;
	for (; count < nex.refEntitiesCount; count++) {
		refEntity_t* refEnt = &nex.refEntities[count];
		int entityNum = refEnt->entityNum;

		// If valid player
		if ((nex.cg_entities[entityNum].currentState->eType == ET_PLAYER)
				&& (entityNum < MAX_CLIENTS)
				&& nex.client[entityNum].infoValid
				&& (entityNum != nex.cg_snap->ps.clientNum)
				// If not dead
				&& !(nex.cg_entities[entityNum].currentState->eFlags & EF_DEAD)
				// If not vulnerable
				&& (!(nex.cg_entities[entityNum].currentState->powerups & (1 << PW_INVULNERABLE)))
				// If body
				&& refEnt->torsoFrameModel) {
			if (IS_PLAYER_ENEMY(entityNum))
				refEnt->customShader = enemyShader;
			else
				refEnt->customShader = friendShader;

			refEnt->renderfx |= RF_DEPTHHACK | RF_NOSHADOW;
		}
		orig_syscall(CG_R_ADDREFENTITYTOSCENE, refEnt);
	}

	// Adjust refDef from 640
	refDef->x = x * nex.cgs_screenXScale;
	refDef->y = y * nex.cgs_screenYScale;
	refDef->width = width * nex.cgs_screenXScale;
	refDef->height = height * nex.cgs_screenYScale;
	
	// Draw the spycam scene
	nexDraw.drawFillRect(x, y, width, height, colorBlack);
	orig_syscall(CG_R_RENDERSCENE, refDef);

	// Draw border
	nexDraw.drawRect(x, y, width, height, colorBlack, 1);

	// Draw the crosshair
	#define CROSSHAIR_SIZE 4
	if (crosshair)
		nexDraw.drawRect(x + (width / 2) - (CROSSHAIR_SIZE / 2), y + (height / 2) - (CROSSHAIR_SIZE / 2) , CROSSHAIR_SIZE, CROSSHAIR_SIZE, colorRed, 1);
}

void CNexSpycams::mirror() {
	#define MIRRORCAM_WIDTH 640.0f
	#define MIRRORCAM_HEIGHT 100.0f
	// Set the view
	refdef_t camRefDef;
	memcpy(&camRefDef, &nex.cg_refdef, sizeof(refdef_t));
	// fov
	camRefDef.fov_x = 90.0f;
	camRefDef.fov_y = 90.0f * (MIRRORCAM_HEIGHT / MIRRORCAM_WIDTH);
	// view angle
	vec3_t camAngle;
	VectorCopy(nex.cg_refdefViewAngles, camAngle);
	camAngle[YAW] -= 180.0f;	// Look behind
	AnglesToAxis(camAngle, camRefDef.viewaxis);

	// Draw the spycam
	drawSpycam(0, 0, MIRRORCAM_WIDTH - 1, MIRRORCAM_HEIGHT - 1, &camRefDef, nex.media.enemyMortarShader, nex.media.friendMortarShader, qfalse);
}

void CNexSpycams::mortar() {
// Set mortar trajectory from current view
	vec3_t angles, forward;
	VectorCopy(nex.cg_refdef.viewaxis[ROLL], forward);
	VectorCopy(nex.cg_snap->ps.viewangles, angles);
	angles[PITCH] -= 60.f;
	AngleVectors(angles, forward, NULL, NULL);
	forward[0] *= 3000 * 1.1f;
	forward[1] *= 3000 * 1.1f;
	forward[2] *= 1500 * 1.1f;

	trajectory_t mortarTrajectory;
	mortarTrajectory.trType = TR_GRAVITY;
	mortarTrajectory.trTime = nex.cg_time;
	VectorCopy(nex.muzzle, mortarTrajectory.trBase);
	VectorCopy(forward, mortarTrajectory.trDelta);

	// Calculate mortar impact
	int timeOffset = 0;
	trace_t mortarTrace;
	vec3_t mortarImpact;
	VectorCopy(mortarTrajectory.trBase, mortarImpact);
	#define TIME_STEP 20
	while (timeOffset < 10000) {
		vec3_t nextPos;
		timeOffset += TIME_STEP;
		BG_EvaluateTrajectory(&mortarTrajectory, nex.cg_time + timeOffset, nextPos, qfalse, 0);
		nex.orig.CG_Trace(&mortarTrace, mortarImpact, 0, 0, nextPos, nex.cg_snap->ps.clientNum, MASK_MISSILESHOT);
		if ((mortarTrace.fraction != 1)
				// Stop if we hit sky
				&& !((mortarTrace.surfaceFlags & SURF_NODRAW) || (mortarTrace.surfaceFlags & SURF_NOIMPACT))
				&& (mortarTrace.contents != 0)) {
			break;
		}
		VectorCopy(nextPos, mortarImpact);
	}

#define MCAM_X 10.0f
#define MCAM_Y 10.0f
#define MCAM_W 320.0f
#define MCAM_H 320.0f

	refdef_t camRefDef;
	memcpy(&camRefDef, &nex.cg_refdef, sizeof(refdef_t));

	camRefDef.fov_x = 90;
	camRefDef.fov_y = 90;

	VectorCopy(mortarImpact, camRefDef.vieworg);
	camRefDef.vieworg[ROLL] += nexValue[VAR_MORTARCAM_H];	// Up the cam
	vec3_t camAngle;
	VectorCopy(nex.cg_refdefViewAngles, camAngle);
	camAngle[PITCH] = 90.0f;	// Look down
	AnglesToAxis(camAngle, camRefDef.viewaxis);
	
	
	nexSpycam.drawSpycam(MCAM_X, MCAM_Y, MCAM_W, MCAM_H, &camRefDef, nex.media.enemyMortarShader, nex.media.friendMortarShader, qtrue);

	// Draw impact time	
	char str[64];
	sprintf_s(str, sizeof(str), "^0[ ^3Impact in:^2 %.1f ^3seconds ^0]", (float)timeOffset / 1000.0f);
	nexDraw.drawText(MCAM_X + (MCAM_W - nexDraw.sizeTextWidth(0.19f, str, &nex.media.mainFont)) / 2, MCAM_Y + MCAM_H - 15, 0.19f, str, 
colorWhite, qfalse, qtrue, &nex.media.mainFont);
}

void CNexSpycams::rifle() {
	vec3_t angles, forward, viewpos;

	trajectory_t rifleTrajectory;
	rifleTrajectory.trType = TR_GRAVITY;
	rifleTrajectory.trTime = nex.cg_time;
	VectorCopy(nex.muzzle, rifleTrajectory.trBase);
	VectorCopy(rifleTrajectory.trBase, viewpos);

	VectorCopy(nex.cg_snap->ps.viewangles, angles);
	AngleVectors(angles, forward, NULL,NULL);
	viewpos[2] += nex.cg_snap->ps.viewheight;
	VectorMA(viewpos, 32, forward, viewpos);
	VectorScale(forward, 2000, forward);

	VectorCopy(forward, rifleTrajectory.trDelta);
	SnapVector(rifleTrajectory.trDelta);

	// Calculate rifle impact
	int timeOffset = 0;
	trace_t rifleTrace;
	vec3_t rifleImpact;
	VectorCopy(rifleTrajectory.trBase, rifleImpact);
	#define TIME_STEPP 50
	int maxTime = 3250;
	int totalFly = 0;

	while (timeOffset < maxTime) {
		vec3_t nextPos;
		timeOffset += TIME_STEPP;
		totalFly += TIME_STEPP;

		BG_EvaluateTrajectory(&rifleTrajectory, nex.cg_time + timeOffset, nextPos, qfalse, 0);
		nex.orig.CG_Trace(&rifleTrace, rifleImpact, 0, 0, nextPos, nex.cg_snap->ps.clientNum, MASK_MISSILESHOT);

		// check for hit 
		if (rifleTrace.startsolid || rifleTrace.fraction != 1) {
			// When a nade flies for over 750ms and hits somnexing, it'll explode
			if (totalFly > 750)
				break;

			// calculate reflect angle (forward axis)
			int hitTime = nex.cg_time + totalFly - TIME_STEPP + TIME_STEPP * rifleTrace.fraction;
			BG_EvaluateTrajectoryDelta(&rifleTrajectory, hitTime, rifleTrajectory.trDelta, qfalse, 0);
			float dot = DotProduct(rifleTrajectory.trDelta, rifleTrace.plane.normal);
			VectorMA(rifleTrajectory.trDelta, -2*dot, rifleTrace.plane.normal, rifleTrajectory.trDelta);

			VectorScale(rifleTrajectory.trDelta, 0.35, rifleTrajectory.trDelta);

			if (rifleTrace.surfaceFlags == 0)
				VectorScale(rifleTrajectory.trDelta, 0.5, rifleTrajectory.trDelta);

			// calc new max time and reset trTime
			maxTime -= timeOffset;
			timeOffset = 0;
			rifleTrajectory.trTime = nex.cg_time;

			// new base origins 
			VectorCopy(rifleTrace.endpos, rifleTrajectory.trBase);
			
			SnapVector(rifleTrajectory.trDelta);
			SnapVector(rifleTrajectory.trBase);
		} else {
			VectorCopy(nextPos, rifleImpact);
       		#define POINT_SIZE 2
			int x, y;
       		if (nexDraw.worldToScreen(nextPos, &x, &y)) {
		  		if (nexEngine.isVisible(nextPos))
					nexDraw.drawFillRect(x - (POINT_SIZE / 2), y - (POINT_SIZE / 2), POINT_SIZE, POINT_SIZE, colorYellow);
				else 
					nexDraw.drawFillRect(x - (POINT_SIZE / 2), y - (POINT_SIZE / 2), POINT_SIZE, POINT_SIZE, colorRed);
			}
		}
	}

	// Set the view
	refdef_t camRefDef = nex.cg_refdef;
	// fov
	camRefDef.fov_x = 90;
	camRefDef.fov_y = 90;
	// origin
	VectorCopy(rifleImpact, camRefDef.vieworg);
	camRefDef.vieworg[ROLL] += nexValue[VAR_RIFLECAM_H];		// Up the cam
	// angle
	vec3_t camAngle;
	VectorCopy(nex.cg_refdefViewAngles, camAngle);
	camAngle[PITCH] = 90;	// Look down
	AnglesToAxis(camAngle, camRefDef.viewaxis);

	// Draw the riflecam
	drawSpycam(10, 10, 250, 250, &camRefDef, nex.media.enemyMortarShader, nex.media.friendMortarShader, qtrue);

	// Draw impact time	
	char str[12];
	sprintf_s(str, sizeof(str), "^2%.1f ^3s", (float)totalFly / 1000.0f);
	nexDraw.drawText(10 + (250 - nexDraw.sizeTextWidth(0.19f, str, &nex.media.mainFont)) / 2, 10 + 250 - 15, 0.19f, str, colorWhite, qfalse, qtrue, &nex.media.mainFont);
}

void CNexSpycams::panzer() {
	// Set panzer direction
	vec3_t forward;
	AngleVectors(nex.cg_snap->ps.viewangles, forward, NULL, NULL );

	// Set a far away end point
	vec3_t aheadPos;
	VectorMA(nex.muzzle, 65536.0f, forward, aheadPos);
	
	// Find panzer impact
	trace_t panzerTrace;
	nex.orig.CG_Trace(&panzerTrace, nex.muzzle, NULL, NULL, aheadPos, nex.cg_snap->ps.clientNum, MASK_MISSILESHOT);
	
	// If no impact or hit sky don't draw cam
	if ((panzerTrace.fraction == 1.0f) || (panzerTrace.surfaceFlags & SURF_NOIMPACT))
		return;

	// Set the view
	refdef_t camRefDef;
	memcpy(&camRefDef, &nex.cg_refdef, sizeof(refdef_t));
	// fov
	camRefDef.fov_x = 30;
	camRefDef.fov_y = 30;
	// angle
	AnglesToAxis(nex.cg_refdefViewAngles, camRefDef.viewaxis);
	VectorCopy(panzerTrace.endpos, camRefDef.vieworg);
	// origin
	vec3_t stepBack;
	VectorMA(vec3_origin, 400, forward, stepBack);
	VectorSubtract(camRefDef.vieworg, stepBack, camRefDef.vieworg);
	
	// Draw the panzer cam
	drawSpycam(10, 10, 160, 160, &camRefDef, nex.media.enemyVisibleShader, nex.media.friendVisibleShader, qtrue);
}

void CNexSpycams::satchel() {
	int satchelEntityNum = nexEngine.findSatchel();
	
	// If don't find satchel
	if (satchelEntityNum == -1)
		return;

	// Set the view
	refdef_t camRefDef;
	memcpy(&camRefDef, &nex.cg_refdef, sizeof(refdef_t));
	// fov
	camRefDef.fov_x = 60;
	camRefDef.fov_y = 60;
	// origin
	VectorCopy(nex.entities[satchelEntityNum].origin, camRefDef.vieworg);
	camRefDef.vieworg[ROLL] += nexValue[VAR_SATCHELCAM_H];		// Up the cam
	// view angle
	vec3_t camAngle;
	VectorCopy(nex.cg_refdefViewAngles, camAngle);
	camAngle[PITCH] = 90;	// Look down
	AnglesToAxis(camAngle, camRefDef.viewaxis);

	// Draw the spycam
	drawSpycam(10, 10, 240, 240, &camRefDef, nex.media.enemyVisibleShader, nex.media.friendVisibleShader, qfalse);
}

