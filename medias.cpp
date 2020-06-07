// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

void CNexEngine::registerMedias() {
	// class esp icons
	nex.media.classIcons[PC_SOLDIER] = nexSyscall.R_RegisterShaderNoMip("gfx/limbo/ic_soldier");
	nex.media.classIcons[PC_MEDIC] = nexSyscall.R_RegisterShaderNoMip("gfx/limbo/ic_medic");
	nex.media.classIcons[PC_ENGINEER] = nexSyscall.R_RegisterShaderNoMip("gfx/limbo/ic_engineer");
	nex.media.classIcons[PC_FIELDOPS] = nexSyscall.R_RegisterShaderNoMip("gfx/limbo/ic_fieldops");
	nex.media.classIcons[PC_COVERTOPS] = nexSyscall.R_RegisterShaderNoMip("gfx/limbo/ic_covertops");

	// weapon esp icons
	nex.media.weaponIcons[MOD_BROWNING] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_browning_1_select");
	nex.media.weaponIcons[WP_MORTAR] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_mortar_1_select");
	nex.media.weaponIcons[WP_AKIMBO_SILENCEDLUGER] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	nex.media.weaponIcons[WP_AKIMBO_LUGER] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	nex.media.weaponIcons[WP_AKIMBO_COLT] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	nex.media.weaponIcons[WP_AKIMBO_SILENCEDCOLT] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	nex.media.weaponIcons[WP_NONE] = nexSyscall.R_RegisterShaderNoMip("icons/noammo");
	nex.media.weaponIcons[WP_SMOKE_MARKER] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_smokegrenade_1_select");
	nex.media.weaponIcons[WP_SMOKETRAIL] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_smokegrenade_1_select");
	nex.media.weaponIcons[WP_MAPMORTAR] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_grenade_1_select");
	nex.media.weaponIcons[WP_ARTY] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_syringe_1_select");
	nex.media.weaponIcons[WP_K43_SCOPE] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_mauser_1_select");
	nex.media.weaponIcons[WP_GPG40] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_kar98_1_select");
	nex.media.weaponIcons[WP_M7] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_m1_garand_1_select");
	nex.media.weaponIcons[WP_GARAND_SCOPE] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_mauser_1_select");
	nex.media.weaponIcons[WP_FG42SCOPE] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_fg42_1_select");
	nex.media.weaponIcons[WP_MORTAR_SET] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_mortar_1_select");
	nex.media.weaponIcons[WP_SMOKE_BOMB] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_dynamite_1_select");
	nex.media.weaponIcons[WP_TRIPMINE] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_dynamite_1_select");
	nex.media.weaponIcons[WP_MOBILE_MG42_SET] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_mg42_1_select");
	nex.media.weaponIcons[WP_SILENCED_COLT] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	nex.media.weaponIcons[WP_MEDKIT] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_medheal_select");
	nex.media.weaponIcons[WP_MOBILE_MG42] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_mg42_1_select");
	nex.media.weaponIcons[WP_THOMPSON] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_thompson_1_select");
	nex.media.weaponIcons[WP_KNIFE] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_knife_1_select");
	nex.media.weaponIcons[WP_LUGER] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_luger_1_select");
	nex.media.weaponIcons[WP_MP40] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_MP40_1_select");
	nex.media.weaponIcons[WP_GRENADE_LAUNCHER] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_grenade_1_select");
	nex.media.weaponIcons[WP_PANZERFAUST] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_panzerfaust_1_select");
	nex.media.weaponIcons[WP_FLAMETHROWER] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_flamnexrower_1_select");
	nex.media.weaponIcons[WP_COLT] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_colt_1_select");
	nex.media.weaponIcons[WP_GRENADE_PINEAPPLE] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_pineapple_1_select");
	nex.media.weaponIcons[WP_STEN] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_sten_1_select");
	nex.media.weaponIcons[WP_MEDIC_SYRINGE] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_syringe_1_select");
	nex.media.weaponIcons[WP_AMMO] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_ammopack_1_select");
	nex.media.weaponIcons[WP_SILENCER] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_silencer_1_select");
	nex.media.weaponIcons[WP_DYNAMITE] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_dynamite_1_select");
	nex.media.weaponIcons[WP_MEDKIT] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_medheal_1_select");
	nex.media.weaponIcons[WP_BINOCULARS] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_binoculars_1_select");
	nex.media.weaponIcons[WP_PLIERS] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_pliers_1_select");
	nex.media.weaponIcons[WP_KAR98] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_kar98_1_select");
	nex.media.weaponIcons[WP_CARBINE] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_mauser_1_select");
	nex.media.weaponIcons[WP_GARAND] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_m1_garand_1_select");
	nex.media.weaponIcons[WP_LANDMINE] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_landmine_1_select");
	nex.media.weaponIcons[WP_SATCHEL] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_satchel_1_select");
	nex.media.weaponIcons[WP_SATCHEL_DET] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_radio_1_select");
	nex.media.weaponIcons[WP_K43] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_mauser_1_select");
	nex.media.weaponIcons[WP_FG42] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_fg42_1_select");
	nex.media.weaponIcons[WP_MEDIC_ADRENALINE] = nexSyscall.R_RegisterShaderNoMip("icons/iconw_syringe_1_select");

	// chams
	nex.media.shaders[GFX_BLUE][CHAM_QUAKE] = nexSyscall.R_RegisterShader("quakeABlueShader");
	nex.media.shaders[GFX_CYAN][CHAM_QUAKE] = nexSyscall.R_RegisterShader("quakeACyanShader");
	nex.media.shaders[GFX_GREEN][CHAM_QUAKE] = nexSyscall.R_RegisterShader("quakeAGreenShader");
	nex.media.shaders[GFX_RED][CHAM_QUAKE] = nexSyscall.R_RegisterShader("quakeARedShader");
	nex.media.shaders[GFX_TEAL][CHAM_QUAKE] = nexSyscall.R_RegisterShader("quakeATealShader");
	nex.media.shaders[GFX_YELLOW][CHAM_QUAKE] = nexSyscall.R_RegisterShader("quakeAYellowShader");

	nex.media.shaders[GFX_BLUE][CHAM_QUAKEB] = nexSyscall.R_RegisterShader("quakeBBlueShader");
	nex.media.shaders[GFX_CYAN][CHAM_QUAKEB] = nexSyscall.R_RegisterShader("quakeBCyanShader");
	nex.media.shaders[GFX_GREEN][CHAM_QUAKEB] = nexSyscall.R_RegisterShader("quakeBGreenShader");
	nex.media.shaders[GFX_RED][CHAM_QUAKEB] = nexSyscall.R_RegisterShader("quakeBRedShader");
	nex.media.shaders[GFX_TEAL][CHAM_QUAKEB] = nexSyscall.R_RegisterShader("quakeBTealShader");
	nex.media.shaders[GFX_YELLOW][CHAM_QUAKEB] = nexSyscall.R_RegisterShader("quakeBYellowShader");

	nex.media.shaders[GFX_BLUE][CHAM_QUAKEC] = nexSyscall.R_RegisterShader("quakeCBlueShader");
	nex.media.shaders[GFX_CYAN][CHAM_QUAKEC] = nexSyscall.R_RegisterShader("quakeCCyanShader");
	nex.media.shaders[GFX_GREEN][CHAM_QUAKEC] = nexSyscall.R_RegisterShader("quakeCGreenShader");
	nex.media.shaders[GFX_RED][CHAM_QUAKEC] = nexSyscall.R_RegisterShader("quakeCRedShader");
	nex.media.shaders[GFX_TEAL][CHAM_QUAKEC] = nexSyscall.R_RegisterShader("quakeCTealShader");
	nex.media.shaders[GFX_YELLOW][CHAM_QUAKEC] = nexSyscall.R_RegisterShader("quakeCYellowShader");

	nex.media.shaders[GFX_BLUE][CHAM_GLOW] = nexSyscall.R_RegisterShader("glowBlueShader");
	nex.media.shaders[GFX_CYAN][CHAM_GLOW] = nexSyscall.R_RegisterShader("glowCyanShader");
	nex.media.shaders[GFX_GREEN][CHAM_GLOW] = nexSyscall.R_RegisterShader("glowGreenShader");
	nex.media.shaders[GFX_RED][CHAM_GLOW] = nexSyscall.R_RegisterShader("glowRedShader");
	nex.media.shaders[GFX_TEAL][CHAM_GLOW] = nexSyscall.R_RegisterShader("glowTealShader");
	nex.media.shaders[GFX_YELLOW][CHAM_GLOW] = nexSyscall.R_RegisterShader("glowYellowShader");

	nex.media.shaders[GFX_BLUE][CHAM_FLAT] = nexSyscall.R_RegisterShader("flatBlueShader");
	nex.media.shaders[GFX_CYAN][CHAM_FLAT] = nexSyscall.R_RegisterShader("flatCyanShader");
	nex.media.shaders[GFX_GREEN][CHAM_FLAT] = nexSyscall.R_RegisterShader("flatGreenShader");
	nex.media.shaders[GFX_RED][CHAM_FLAT] = nexSyscall.R_RegisterShader("flatRedShader");
	nex.media.shaders[GFX_TEAL][CHAM_FLAT] = nexSyscall.R_RegisterShader("flatTealShader");
	nex.media.shaders[GFX_YELLOW][CHAM_FLAT] = nexSyscall.R_RegisterShader("flatYellowShader");

	nex.media.whiteFlatShader = nexSyscall.R_RegisterShader("flatWhiteShader");
	nex.media.friendMortarShader = nexSyscall.R_RegisterShader("friendMortarShader");
	nex.media.enemyMortarShader = nexSyscall.R_RegisterShader("enemyMortarShader");

	// spree sounds
	// HAD TO DISABLE LOTS OF THEM BECAUSE FOR SOME REASON THEY WOULDN'T
	// GET LOADED (ONLY THE FIRST ONES, SO THE SOUNDS ON BIGGER LEVELS WOULDN'T WORK)
	nex.media.spreeSounds.level1[0] = nexSyscall.S_RegisterSound("sound/spree/level1/doublekill1.wav", qfalse);
	nex.media.spreeSounds.level1[1] = nexSyscall.S_RegisterSound("sound/spree/level1/excelent.wav", qfalse);
	nex.media.spreeSounds.level1[2] = nexSyscall.S_RegisterSound("sound/spree/level1/impressive.wav", qfalse);
//	nex.media.spreeSounds.level1[3] = nexSyscall.S_RegisterSound("sound/spree/level1/killingspree1.wav", qfalse);
//	nex.media.spreeSounds.level1[4] = nexSyscall.S_RegisterSound("sound/spree/level1/killingspree2.wav", qfalse);
//	nex.media.spreeSounds.level1[5] = nexSyscall.S_RegisterSound("sound/spree/level1/perfect.wav", qfalse);
	// level 2
	nex.media.spreeSounds.level2[0] = nexSyscall.S_RegisterSound("sound/spree/level1/doublekill1.wav", qfalse);
	nex.media.spreeSounds.level2[1] = nexSyscall.S_RegisterSound("sound/spree/level2/godlike1.wav", qfalse);
	nex.media.spreeSounds.level2[2] = nexSyscall.S_RegisterSound("sound/spree/level2/godlike2.wav", qfalse);
//	nex.media.spreeSounds.level2[3] = nexSyscall.S_RegisterSound("sound/spree/level2/holyshit.wav", qfalse);
//	nex.media.spreeSounds.level2[4] = nexSyscall.S_RegisterSound("sound/spree/level2/multikill1.wav", qfalse);
//	nex.media.spreeSounds.level2[5] = nexSyscall.S_RegisterSound("sound/spree/level2/multikill2.wav", qfalse);
//	nex.media.spreeSounds.level2[6] = nexSyscall.S_RegisterSound("sound/spree/level2/multikill3.wav", qfalse);
	// level 3
	nex.media.spreeSounds.level3[0] = nexSyscall.S_RegisterSound("sound/spree/level2/tripplekill.wav", qfalse);
	nex.media.spreeSounds.level3[1] = nexSyscall.S_RegisterSound("sound/spree/level3/dominating1.wav", qfalse);
	nex.media.spreeSounds.level3[2] = nexSyscall.S_RegisterSound("sound/spree/level3/dominating2.wav", qfalse);
//	nex.media.spreeSounds.level3[3] = nexSyscall.S_RegisterSound("sound/spree/level3/ultrakill1.wav", qfalse);
//	nex.media.spreeSounds.level3[4] = nexSyscall.S_RegisterSound("sound/spree/level3/ultrakill2.wav", qfalse);
//	nex.media.spreeSounds.level3[5] = nexSyscall.S_RegisterSound("sound/spree/level3/ultrakill3.wav", qfalse);
	// level 4
	nex.media.spreeSounds.level4[0] = nexSyscall.S_RegisterSound("sound/spree/level4/monsterkill1.wav", qfalse);
	nex.media.spreeSounds.level4[1] = nexSyscall.S_RegisterSound("sound/spree/level4/monsterkill2.wav", qfalse);
	nex.media.spreeSounds.level4[2] = nexSyscall.S_RegisterSound("sound/spree/level4/unreal.wav", qfalse);
//	nex.media.spreeSounds.level4[3] = nexSyscall.S_RegisterSound("sound/spree/level4/unstoppable1.wav", qfalse);
//	nex.media.spreeSounds.level4[4] = nexSyscall.S_RegisterSound("sound/spree/level4/unstoppable2.wav", qfalse);
//	nex.media.spreeSounds.level4[5] = nexSyscall.S_RegisterSound("sound/spree/level4/wickedsick.wav", qfalse);
}

