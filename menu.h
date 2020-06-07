// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

// toggle menu
#define NEX_MENU_KEY	VK_F9

// menu look
#define MENU_TEXT_SIZE		0.19f
#define MENU_SCREEN_BORDER	20
#define MENU_ITEM_SPACE		1

// avoid 'flickering'
#define NEX_MENU_KEY_DELAY	250

// all categories
typedef enum {
	CAT_VISUAL,
	CAT_AIMBOT,
	CAT_ESP,
	CAT_MISC,
	CAT_SPYCAM,
	CATS_MAX
} nexMenuCat_t;

// vars
typedef enum {
	// Aimbot
	VAR_AUTO,
	VAR_AIMFOV,
	VAR_STEP,
	VAR_AUTOVECZ,
	VAR_VECZ,
	VAR_VECZ_STEN,
	VAR_VECZ_SCOPE,
	VAR_SNIPERDELAY,
	VAR_OVERHEAT,
	VAR_HUMAN,
	VAR_HUMANVALUE,
	VAR_HEADONLY,
	VAR_AIMPOINT,
	VAR_ENEMYPREDICT,
	VAR_SELFPREDICT,
	
	// Visual
	VAR_ADVERT,
	VAR_SPAWNTIMER,
	VAR_NOWEAPONZOOM,
	VAR_SMOKE,
	VAR_RADAR,

	// Esp
	VAR_WALLHACK,
	VAR_COLORS,
	VAR_CHAMS,
	VAR_ESP,
	VAR_ESP_FADE,
	VAR_ESP_NAME,
	VAR_ESP_CW,
	VAR_ESP_MISSILE,

	// Misc
	VAR_KILLSPAM,

	// Spycam
	VAR_MORTARCAM,
	VAR_MORTARCAM_H,
	VAR_RIFLECAM,
	VAR_RIFLECAM_H,
	VAR_SATCHELCAM,
	VAR_SATCHELCAM_H,
	VAR_PANZERCAM,
	VAR_MIRRORCAM,

	VARS_TOTAL
} nexVarsEnum_t;

// var choices
typedef struct {
	char	*name;
	float	value;
} choice_t;

typedef struct {
	nexMenuCat_t	id;
	char			*name;
} menuCat_t;

#define MAX_CHOICES 20
typedef struct {
	char			*menuText;
	float			defaultValue;
	char			*cvarName;
	nexMenuCat_t	menuCategory;
	bool			forceDefault;
	choice_t		choices[MAX_CHOICES];
} nexVars_t;

// store settings
extern nexVars_t nexVars[VARS_TOTAL];	// contains all the information about vars
extern float nexValue[VARS_TOTAL];		// contains var values

// some menu options
typedef enum {
	SMOKE_NORMAL,
	SMOKE_TRANSPARENT,
	SMOKE_REMOVE
} smokeType_t;

class CNexMenu {
	public:
		void drawMenu();
		void readConfig();
		void writeConfig();
	public:
};

extern CNexMenu nexMenu;
