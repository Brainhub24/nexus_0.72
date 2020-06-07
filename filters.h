// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

/*
==============================
Target filters
==============================
*/

// player
#define TARGET_PLAYER			(1<<0)
#define TARGET_PLAYER_VULN		(1<<1)
#define TARGET_PLAYER_ALIVE		(1<<2)
#define TARGET_PLAYER_DEAD		(1<<3)
// player aim target
#define TARGET_PLAYER_HEAD		(1<<4)
#define TARGET_PLAYER_BODY		(1<<5)
// missile common
#define TARGET_MISSILE			(1<<7)
#define TARGET_MISSILE_ARMED	(1<<8)
#define TARGET_MISSILE_NOTARMED	(1<<9)
// missile
#define TARGET_MISSILE_DYNAMITE	(1<<10)
#define TARGET_MISSILE_LANDMINE	(1<<11)
#define TARGET_MISSILE_ROCKET	(1<<12)
// common
#define TARGET_ENEMY			(1<<13)
#define TARGET_FRIEND			(1<<14)
#define TARGET_VISIBLE			(1<<15)
#define TARGET_NOTVISIBLE		(1<<16)
// helper
#define TARGET_PLAYER_HEAD_BODY TARGET_VISIBLE
#define TARGET_PLAYER_ENEMY	(TARGET_PLAYER | TARGET_PLAYER_VULN | TARGET_ENEMY)

/*
==============================
Target filters
==============================
*/

#define IS_PLAYER_VALID(entityNum) ((entityNum < MAX_CLIENTS) \
		&& (nex.cg_entities[entityNum].currentState->eType == ET_PLAYER) \
		&& nex.client[entityNum].infoValid \
		&& nex.entities[entityNum].isValid \
		&& (nex.cg_clientNum != entityNum))
#define IS_PLAYER_VULNERABLE(entityNum) (IS_PLAYER_VALID(entityNum) \
		&& (!(nex.cg_entities[entityNum].currentState->powerups & (1 << PW_INVULNERABLE))))
#define IS_PLAYER_ENEMY(entityNum) (IS_PLAYER_VALID(entityNum) \
		&& (nex.client[entityNum].team != nex.client[nex.cg_clientNum].team))
#define IS_PLAYER_DEAD(entityNum) (IS_PLAYER_VALID(entityNum) \
		&& (nex.cg_entities[entityNum].currentState->eFlags & EF_DEAD))
#define IS_PLAYER_PRONE(entityNum) (IS_PLAYER_VALID(entityNum) \
		&& (nex.cg_entities[entityNum].currentState.eFlags & EF_PRONE))
#define IS_PLAYER_CROUCH(entityNum) (IS_PLAYER_VALID(entityNum) \
		&& (nex.cg_entities[entityNum].currentState.eFlags & EF_CROUCHING))
// grenade, dynamite, landmine, satchel, ... are missiles
#define IS_MISSILE(entityNum) ((nex.cg_entities[entityNum].currentValid) \
		&& (nex.cg_entities[entityNum].currentState->eType == ET_MISSILE))
#define IS_MISSILE_ARMED(entityNum) (IS_MISSILE(entityNum) \
		&& (nex.cg_entities[entityNum].currentState->teamNum < 4))
#define IS_MISSILE_NOTARMED(entityNum) (IS_MISSILE(entityNum) \
		&& (nex.cg_entities[entityNum].currentState->teamNum > 4))
#define IS_MISSILE_ENEMY(entityNum) (IS_MISSILE(entityNum) \
		&& (IS_MISSILE_NOTARMED(entityNum) ? \
			((nex.cg_entities[entityNum].currentState->teamNum - 4) != nex.client[nex.cg_snap->ps.clientNum].team) : \
			(nex.cg_entities[entityNum].currentState->teamNum != nex.client[nex.cg_snap->ps.clientNum].team)))
#define IS_DYNAMITE(entityNum) (IS_MISSILE(entityNum) \
		&& (nex.cg_entities[entityNum].currentState->weapon == WP_DYNAMITE))
#define IS_LANDMINE(entityNum) (IS_MISSILE(entityNum) \
		&& (nex.cg_entities[entityNum].currentState->weapon == WP_LANDMINE))
// Grenade: axis = LAUNCHER and allies = PINEAPPLE
#define IS_GRENADE(entityNum) (IS_MISSILE(entityNum) \
		&& ((nex.cg_entities[entityNum].currentState->weapon == WP_GRENADE_LAUNCHER) \
			|| (nex.cg_entities[entityNum].currentState->weapon == WP_GRENADE_PINEAPPLE)))
#define IS_MORTAR(entityNum) (IS_MISSILE(entityNum) \
		&& (nex.cg_entities[entityNum].currentState->weapon == WP_MORTAR_SET))
#define IS_PANZER(entityNum) (IS_MISSILE(entityNum) \
		&& (nex.cg_entities[entityNum].currentState->weapon == WP_PANZERFAUST))
#define IS_SATCHEL(entityNum) (IS_MISSILE(entityNum) \
		&& (nex.cg_entities[entityNum].currentState->weapon == WP_SATCHEL))
#define IS_SMOKE(entityNum) (IS_MISSILE(entityNum) \
		&& (nex.cg_entities[entityNum].currentState->weapon == WP_SMOKE_BOMB))
