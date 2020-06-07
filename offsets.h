// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

/*
===============================
ET offsets
===============================
*/

const static nexET_t nexETs[] = {
	{	"2.55",	// Version string
		0xd88f0f0a,	// crc32
		0x41E8A0,	// Cmd_AddCommand
		0x150B1EC,	// cl_mouseDx
		0x150B1E8,	// cl_mouseDy
		0x0,	// serverIP
		0x445810,	// syscall
		0x9FFCC8,	// fs_searchpaths
		0x427700,	// FS_PureServerSetLoadedPaks
	},
	{	"2.56",	// Version string
		0x556af704,	// crc32
		0x41E890,	// Cmd_AddCommand
		0x15086EC,	// cl_mouseDx
		0x15086E8,	// cl_mouseDy
		0x0,	// serverIP
		0x445DA0,	// syscall
		0x9FCBD8,	// fs_searchpaths
		0x41E770,	// FS_PureServerSetLoadedPaks
	},
	{	"2.60",	// Version string
		0x539e3563,	// crc32
		0x41FD20,	// Cmd_AddCommand
		0x13EEA8C,	// cl_mouseDx
		0x13EEA88,	// cl_mouseDy
		0x165F7C8,	// serverIP
		0x448780,	// syscall
		0x9DAA94,	// fs_searchpaths
		0x429620,	// FS_PureServerSetLoadedPaks
	},
	{	"2.60b",	// Version string
		0x4BC60DD5,	// crc32
		0x41FD20,	// Cmd_AddCommand
		0x13EEA8C,	// cl_mouseDx
		0x13EEA88,	// cl_mouseDy
		0x165F7C8,	// serverIP
		0x4488A0,	// syscall
		0x9DAA94,	// fs_searchpaths
		0x429620,	// FS_PureServerSetLoadedPaks
	},
};

/*
===============================
Mod offsets
===============================
*/

const static nexMod_t nexMods[] = {
	{	"etpro", "3.2.6",	// name & version
		MOD_ETPRO, 0xc54721af,	// type & crc32
		0x37D6FD0,	2700,	// cg_entities

		0x83610,			// CG_Trace
		0x80CA0,			// CG_DamageFeedback
		0xA5720,			// CG_FinishWeaponChange
		0x535C0,			// CG_EntityEvent
		0x9CFC0,			// CG_DrawActiveFrame
		0x6C2D0,			// CG_Init

		// for etpro
		0x6B1B0,			// CG_ConfigString()
		0xF2D50,			// syscallptr
		0x1BA0,			// anticheat
	},
	{	"etpro", "3.2.5",	// name & version
		MOD_ETPRO, 0xfaf0bbfe,	// type & crc32
		0x37D5F10,	2700,	// cg_entities

		0x83060,			// CG_Trace
		0x80760,			// CG_DamageFeedback
		0xA5170,			// CG_FinishWeaponChange
		0x53080,			// CG_EntityEvent
		0x9CA10,			// CG_DrawActiveFrame
		0x6BD90,			// CG_Init

		// for etpro
		0x6AC70,			// CG_ConfigString()
		0xF2D30,			// syscallptr
		0x1720,			// anticheat
	},
	{	"etpro", "3.2.4",	// name & version
		MOD_ETPRO, 0x735219ff,	// type & crc32
		0x37D5F10,	2700,	// cg_entities
		0x83060,			// CG_Trace
		0x80760,			// CG_DamageFeedback
		0xA5170,			// CG_FinishWeaponChange
		0x53080,			// CG_EntityEvent
		0x9CA10,			// CG_DrawActiveFrame
		0x6BD90,			// CG_Init

		// for etpro
		0x6AC70,			// CG_ConfigString()
		0xF2D30,			// syscallptr
		0x1720,			// anticheat
	},
	{	"etmain", "2.60",	// name & version
		MOD_ETMAIN, 0x55e0c0bc,	// type & crc32
		0x35945C0,	2676,	// cg_entities

		0x49080,			// CG_Trace
		0x47500,			// CG_DamageFeedback
		0x5DFB0,			// CG_FinishWeaponChange
		0x2C440,			// CG_EntityEvent
		0x59D60,			// CG_DrawActiveFrame
		0x3D3E0,			// CG_Init
	},
	{	"etpub", "20060818",	// name & version
		MOD_ETPUB, 0x8fe73e0c,	// type & crc32
		0x35C1FA0,	2700,	// cg_entities

		0x53160,			// CG_Trace
		0x51750,			// CG_DamageFeedback
		0x6B750,			// CG_FinishWeaponChange
		0x2F540,			// CG_EntityEvent
		0x65030,			// CG_DrawActiveFrame
		0x451D0,			// CG_Init
	},
	{	"etmain", "2.55",	// name & version
		MOD_ETMAIN, 0xffdda6c7,	// type & crc32
		0x358B6E0,	2676,	// cg_entities

		0x49680,			// CG_Trace
		0x47D80,			// CG_DamageFeedback
		0x60620,			// CG_FinishWeaponChange
		0x2C503,			// CG_EntityEvent
		0x5A270,			// CG_DrawActiveFrame
		0x3B980,			// CG_Init
	},
	{	"jaymod", "2.0.1",	// name & version
		MOD_JAYMOD, 0x0d1bc3cb,	// type & crc32
		0x0,	2704,	// cg_entities
	},
	{	"jaymod", "2.0.2",	// name & version
		MOD_JAYMOD, 0x237467f9,	// type & crc32
		0x0,	2704,	// cg_entities
	},
	{	"jaymod", "2.0.3",	// name & version
		MOD_JAYMOD, 0xcfe8f001,	// type & crc32
		0x0,	2704,	// cg_entities
	},
};

// centity_t struct offsets
#define CG_ENTITIES_CURRENTSTATE_OFFSET 0x0
#define CG_ENTITIES_CURRENTVALID_OFFSET 0x244