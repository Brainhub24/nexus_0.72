// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

/*
===============================
Some enums/typedefs/defines
===============================
*/

#define MAX_PREFIX_LENGTH	4	// max prefix length
#define ETPRO_GUID_SIZE		40	// max etpro guid length

typedef int socket_t; // sokkets
#define CTHREAD(thread) (CreateThread(0, 0, (LPTHREAD_START_ROUTINE)thread, 0, 0, 0))

/*
==============================
Chams/Esp/Colors
==============================
*/

typedef enum {
	GFX_NONE,
	GFX_RED,
	GFX_BLUE,
	GFX_GREEN,
	GFX_CYAN,
	GFX_TEAL,
	GFX_YELLOW,
	GFXS_TOTAL
} nexGfxColors_t;

typedef enum {
	CHAM_GLOW = 1,
	CHAM_GLOWONLY,
	CHAM_FLAT,
	CHAM_QUAKE,
	CHAM_QUAKEB,
	CHAM_QUAKEC,
	CHAMS_TOTAL
} nexChams_t;

typedef struct {
	nexGfxColors_t	chams;
	float			*esp;	// vec3_t
} nexColorModel_t;

typedef struct {
	nexColorModel_t friendVisible;
	nexColorModel_t friendNotVisible;
	nexColorModel_t enemyVisible;
	nexColorModel_t enemyNotVisible;
} nexPlayerColor_t;

/*
===============================
Sounds
===============================
*/

typedef struct {
	sfxHandle_t level1[3];
	sfxHandle_t level2[3];
	sfxHandle_t level3[3];
	sfxHandle_t level4[3];
} nexSpreeSounds_t;

/*
===============================
Shaders we're registering/using
===============================
*/

typedef struct {
	// 2d images
	qhandle_t	whiteShader;			// used for drawing 2d stuff (radar, advert, etc)
	qhandle_t	cursorIcon;				// need cursor icon for menu
	qhandle_t	bg_alliedpin;			// for irc fireteam
	qhandle_t	bg_axispin;				// for irc fireteam

	// other shaders
	qhandle_t	smokePuffshader;		// removal (or transparenter)
	qhandle_t	reticleShaderSimple;	// removal
	qhandle_t	binocShaderSimple;		// removal

	// shaders used for chams
	qhandle_t	whiteFlatShader;
	qhandle_t	friendVisibleShader;
	qhandle_t	friendNotVisibleShader;
	qhandle_t	enemyVisibleShader;
	qhandle_t	enemyNotVisibleShader;
	qhandle_t	shaders[GFXS_TOTAL][CHAMS_TOTAL];
	qhandle_t	friendMortarShader;
	qhandle_t	enemyMortarShader;

	// colors
	vec3_t		friendVisibleColor;
	vec3_t		friendNotVisibleColor;
	vec3_t		enemyVisibleColor;
	vec3_t		enemyNotVisibleColor;

	// models
	qhandle_t	hHead;
	qhandle_t	hGlasses;

	// esp/radar icons
	qhandle_t	weaponIcons[WP_NUM_WEAPONS];		// weapons
	qhandle_t	classIcons[NUM_PLAYER_CLASSES];		// classes

	// sounds
	sfxHandle_t	limboSelect;			// so we can play sound on mouse click
	nexSpreeSounds_t	spreeSounds;

	// fonts
	fontInfo_t	mainFont;				// use 2 different fonts because it makes it look a bit better
	fontInfo_t	altFont;				// than having only one type of font on screen :P
} nexMedia_t;

/*
===============================
We will need some sockets
===============================
*/

typedef struct {
	socket_t irc;	// irc chat
	socket_t php;	// php tracker
	socket_t spc;	// spec list
	socket_t det;	// detection status check
} nexSockets_t;

/*
===================
Call original cgame funcs
===================
*/

// CG_FinishWeaponChange
typedef void (__cdecl *CG_FinishWeaponChange_t)(int lastweap, int newweap);
void __cdecl nex_CG_FinishWeaponChange(int lastweap, int newweap);

// CG_Init
typedef void (__cdecl *CG_Init_t)(int serverMessageNum, int serverCommandSequence, int clientNum);
void nex_CG_Init(int serverMessageNum, int serverCommandSequence, int clientNum);

// CG_DrawActiveFrame
typedef void (__cdecl *CG_DrawActiveFrame_t)(int serverTime, int stereoView, qboolean demoPlayback);
void nex_CG_DrawActiveFrame(int serverTime, int stereoView, qboolean demoPlayback);

// CG_ConfigString
typedef const char* (__cdecl *CG_ConfigString_t)(int index);
const char* __cdecl nex_CG_ConfigString(int index);

// CG_Trace
typedef void (__cdecl *CG_Trace_t)(trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask);

// CG_DamageFeedback
typedef void (__cdecl *CG_DamageFeedback_t)(int yawByte, int pitchByte, int damage);
void __cdecl nex_CG_DamageFeedback(int, int, int);

// CG_FinishWeaponChange
typedef void (__cdecl *CG_FinishWeaponChange_t)(int lastweap, int newweap);
void __cdecl nex_CG_FinishWeaponChange(int lastweap, int newweap);

// CG_EntityEvent
typedef void (__cdecl *CG_EntityEvent_t)(centity_t *cent, vec3_t position);
void __cdecl nex_CG_EntityEvent(centity_t *cent, vec3_t position);

/*
===================
Multi mod support
===================
*/

typedef struct {
	// for all mods
	CG_Trace_t				CG_Trace;
	CG_DamageFeedback_t		CG_DamageFeedback;
	CG_FinishWeaponChange_t CG_FinishWeaponChange;
	CG_EntityEvent_t		CG_EntityEvent;
	CG_Init_t				CG_Init;
	CG_DrawActiveFrame_t	CG_DrawActiveFrame;

	// only in etpro
	CG_ConfigString_t		CG_ConfigString;
} nexModOrigs_t;

typedef enum {
	MOD_ETMAIN,
	MOD_ETPUB,
	MOD_JAYMOD,
	MOD_NOQUARTER,
	MOD_ETPRO,
	MOD_TCE
} nexModType_t;

typedef struct {
	char	*name;
	char	*version;
	byte	type;
	u_long	crc32;

	// cg_entities
	DWORD	cg_entities;
	int		centity_t_size;

	// all mods
	DWORD	CG_Trace;
	DWORD	CG_DamageFeedback;
	DWORD	CG_FinishWeaponChange;
	DWORD	CG_EntityEvent;
	DWORD	CG_DrawActiveFrame;
	DWORD	CG_Init;

	// hooking these only in etpro
	DWORD	CG_ConfigString;
	DWORD	syscallptr;
	DWORD	anticheat;
} nexMod_t;

/*
==============================
entity info
==============================
*/

typedef struct {
	bool	isValid;
	bool	isInScreen;
	bool	isVisible;		// true if body or head is visible
	bool	isHeadVisible;
	
	float	*espColor;		// vec4_t

	int		screenX;
	int		screenY;

	float	distance;
	vec3_t	origin;			// Body origin
	vec3_t	bodyPart;		// vec3_origin if no body parts visible
	vec3_t	head;
	vec3_t	headAxis[3];
} nexEntity_t;

typedef struct {
	entityState_t	*currentState;
	qboolean		*currentValid;
	refEntity_t		pe_bodyRefEnt;
} nex_centity_t;

/*
==============================
client info
==============================
*/

typedef struct {
	bool	infoValid;
	char	name[MAX_QPATH];
	char	cleanName[MAX_QPATH];
	int		team;
	int		cls;
	bool	ref;
} nexClientInfo_t;

/*
==============================
Some stats
==============================
*/

typedef struct {
	int	refCount;
	int	killCount;
	int	killSpreeCount;
	int	killCountNoDeath;
	int	firstKillSpreeTime;
	int	lastVictim;
	int	lastKillTime;
} nexStats_t;

/*
===============================
Main nex struct - ZERO'ED every CG_SHUTDOWN
===============================
*/

typedef struct {
	// anti etpro AC stuff - credits to sinner
	HANDLE	hHideFromAC;

	// mod support
	const nexMod_t	*mod;				// contains offsets of the mod we're playing atm
	nexModOrigs_t	orig;				// call some cgame funcs

	// cg infos
	int	cg_clientNum;					// clientNum retreived from CG_INIT
	int cg_time;						// server time, set every frame, retreived from CG_DRAW_ACTIVE_FRAME
	int	cg_frametime;					// time needed to draw this frame

	// glconfig
	float cgs_screenXScale;				// X&Y scales, used for drawing stuff based
	float cgs_screenYScale;				// on 640*480 resolution
	glconfig_t	cgs_glconfig;			// contains some rendering info

	// calculate some shyt
	int	cgs_levelStartTime;					// some stuff needed to calculate spawntimes
	int cgs_aReinfOffset[TEAM_NUM_TEAMS];	// or some other things that need this info (i.e. chargetimes)

	// default 2.60 struct(s)
	snapshot_t	*cg_snap;				// snapshot info

	// medias
	nexMedia_t	media;					// medias (images, various shaders, sounds, fonts, etc)

	// sockets
	nexSockets_t	socket;				// our sockets

	// mouse pointer
	int	mouse[2];						// X&Y positon of mouse ptr on our screen

	// clients
	nexClientInfo_t	client[MAX_CLIENTS];

	// cg_entities
	nex_centity_t cg_entities[MAX_GENTITIES];

	// entities - ZERO'ED EVERY CG_DRAW_ACTIVE_FRAME
	nexEntity_t	entities[MAX_GENTITIES];
	refEntity_t	refEntities[MAX_GENTITIES];
	int			refEntitiesCount;

	// refdef
	refdef_t	cg_refdef;
	vec3_t		cg_refdefViewAngles;

	// menu
	bool		isMeunOpen;

	// gameState(s)
	gameState_t	*cgs_gameState;
	gamestate_t	cgs_gamestate;

	// stats
	nexStats_t	stats;

	// limbo times
	int			cg_redlimbotime;
	int			cg_bluelimbotime;

	// muzzle
	vec3_t		muzzle;						// we're not firing from vieworg, this is a correction
} nex_t;

/*
===================
Multi ET support
===================
*/

typedef struct {
	char	*version;
	u_long	crc32;

	DWORD	Cmd_AddCommand;
	DWORD	cl_mouseDx;
	DWORD	cl_mouseDy;
	DWORD	serverIP;
	DWORD	syscall;
	DWORD	fs_searchpaths;
	DWORD	FS_PureServerSetLoadedPaks;
} nexET_t;

/*
===============================
Static nex struct - never ZERO'ED
===============================
*/

typedef struct {
	// some paths
	char	path[MAX_OSPATH];			// c:\path\to\nexus\ 
	char	etPath[MAX_OSPATH];			// c:\path\to\et\ 
	char	cfgFilename[MAX_OSPATH];	// c:\path\to\et\etmain\campaigncycle.cfg

	// prefix
	char	prefix[MAX_PREFIX_LENGTH];	// prefix for console cmds

	// custom pk3 name
	char			pk3Name[MAX_OSPATH];

	// etpro
	bool	etproSpoof;
	char	etproGuid[ETPRO_GUID_SIZE];
	DWORD	syscallptr;

	// logging
	#ifdef NEX_LOGGING
		FILE	*logFile;					// ptr to logfile
	#endif

	// some modules
	HMODULE	cgHandle;					// cgame.dll
	HMODULE	etHandle;					// ET.exe

	// multi ET version support
	bool	etSupported;				// don't hook stuff if ET version ain't supported
	const nexET_t	*ET;				// contains offsets of the ET version we're using

	// Hook status
	bool	etHooked;
	bool	cgHooked;

	// aim key
	char	aimKey[24];					// bind X "+aimbot"

	// killspam
	char	ksFormat[256];
} sNex_t;

