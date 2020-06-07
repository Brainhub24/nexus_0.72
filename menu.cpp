// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

CNexMenu nexMenu;

// Categories text definitions
menuCat_t menuCats[] = {
	{ CAT_AIMBOT,	"--- AIMBOT ---" },
	{ CAT_VISUAL,	"--- VISUAL ---" },
	{ CAT_ESP,		"--- ESP ---" },
	{ CAT_MISC,		"--- MISC ---" },
	{ CAT_SPYCAM,	"--- SPYCAM ---" },
};

// store var values
float nexValue[VARS_TOTAL];

// define all vars - WATCH OUT ON ORDER!!!
nexVars_t nexVars[VARS_TOTAL] = {
	// aimbot
	{ "Automatic",			false,	"auto",		CAT_AIMBOT,	false,	{ { "Off", false }, { "On", true } } },
	{ "Aimbot FOV",			360.0f,	"aimfov",	CAT_AIMBOT,	false,	{} },
	{ "Add/dec by value",	1.0f ,	"step",		CAT_AIMBOT,	false,	{ { "10.0", 10.0f }, { "1.0", 1.0f }, { "0.1", 0.1f }, { "0.01", 0.01f } } },
	{ "Auto vecZ",			false,	"autovec",	CAT_AIMBOT,	false,	{ { "Off", false }, { "On", true } } },
	{ "All weapons vecZ",	5.5f,	"vecz",		CAT_AIMBOT,	false,	{} },
	{ "Sten vecZ",			5.5f,	"veczsten",	CAT_AIMBOT,	false,	{} },
	{ "Scoped vecZ",		5.5f,	"veczscop",	CAT_AIMBOT,	false,	{} },
	{ "Sniper delay",		400.0f,	"sniperd",	CAT_AIMBOT,	false,	{} },
	{ "Anti overheat",		true,	"overheat",	CAT_AIMBOT,	false,	{ { "Off", false }, { "On", true } } },
	{ "Human aim",			false,	"human",	CAT_AIMBOT,	false,	{ { "Off", false }, { "On", true } } },
	{ "Human value",		0.15f,	"humanval",	CAT_AIMBOT,	false,	{} },
	{ "Head only",			false,	"headonly",	CAT_AIMBOT,	false,	{ { "Off", false }, { "On", true } } },
	{ "Draw aim point",		false,	"aimpt",	CAT_AIMBOT,	false,	{ { "Off", false }, { "On", true } } },
	{ "Enemy prediction",	false,	"epredict",	CAT_AIMBOT,	false,	{ { "Off", false }, { "On", true } } },
	{ "Self prediction",	true,	"spredict",	CAT_AIMBOT,	false,	{ { "Off", false }, { "On", true } } },

	// visuals
	{ "Info bar",				true,	"advert",	CAT_VISUAL,	false,	{ { "Off", false }, { "On", true } } },
	{ "Spawntimer",			true,	"spawn",	CAT_VISUAL,	false,	{ { "Off", false }, { "On", true } } },
	{ "Remove weapon zoom",	true,	"wpnzoom",	CAT_VISUAL,	false,	{ { "Off", false }, { "On", true } } },
	{ "Smoke",	SMOKE_TRANSPARENT,	"smoke",	CAT_VISUAL, false,	{ { "Leave normal", SMOKE_NORMAL }, { "Make transparent", SMOKE_TRANSPARENT }, { "Remove completely", SMOKE_REMOVE } } },
	{ "Radar",				true,	"radar",	CAT_VISUAL,	false,	{ { "Off", false }, { "On", true } } },

	// esp
	{ "Wallhack",			true,	"wallhack",	CAT_ESP,	false,	{ { "Off", false }, { "On", true } } },
	{ "Colors",				0,		"colors",	CAT_ESP,	0,		{ { "Type 1", 0 }, { "Type 2", 1 }, { "Type 3", 2 } } },
	{ "Chams",			CHAM_QUAKE,	"chams",	CAT_ESP,	false,	{ { "Off", false }, { "Glow", CHAM_GLOW }, { "Glow only", CHAM_GLOWONLY }, { "Flat", CHAM_FLAT } , { "QuakeIII A", CHAM_QUAKE } , { "QuakeIII B", CHAM_QUAKEB }, { "QuakeIII C", CHAM_QUAKEC } } },
	{ "ESP",				true,	"esp",		CAT_ESP,	false,	{ { "Off", false }, { "On", true } } },
	{ "ESP fade",			true,	"esp_fade",	CAT_ESP,	false,	{ { "Off", false }, { "On", true } } },
	{ "Name ESP",			true,	"esp_name",	CAT_ESP,	false,	{ { "Off", false }, { "On", true } } },
	{ "Class & Weapon ESP",	false,	"esp_cw",	CAT_ESP,	false,	{ { "Off", false }, { "On", true } } },
	{ "Entity ESP",		    true,	    "esp_ms",	CAT_ESP,	false,	{ { "Off", false }, { "On", true } } },

	// misc
	{ "Kill spam",			false,	"kspam",	CAT_MISC,	false,	{ { "Off", false }, { "On", true } } },

	// Spycams
	{ "Mortar cam",			true,	"mcam",		CAT_SPYCAM,	false,	{ { "Off", false }, { "On", true } } },
	{ "Mortar cam height",	600.0f,	"mcam_h",	CAT_SPYCAM, false,	{ { "200", 200.0f }, { "300", 300.0f }, { "400", 400.0f }, { "500", 500.0f }, { "600", 600.0f }, { "700", 700.0f }, { "800", 800.0f } } },
	{ "Rifle cam",			true,	"rcam",		CAT_SPYCAM, false,	{ { "Off", false }, { "On", true } } },
	{ "Rifle cam height",	500.0f,	"rcam_h",	CAT_SPYCAM, false,	{ { "200", 200.0f }, { "300", 300.0f }, { "400", 400.0f }, { "500", 500.0f }, { "600", 600.0f }, { "700", 700.0f }, { "800", 800.0f } } },
	{ "Satchel cam",		true,	"scam",		CAT_SPYCAM, false,	{ { "Off", false }, { "On", true } } },
	{ "Satchel cam height",	300.0f,	"scam_h",	CAT_SPYCAM, false,	{ { "200", 200.0f }, { "300", 300.0f }, { "400", 400.0f }, { "500", 500.0f }, { "600", 600.0f }, { "700", 700.0f }, { "800", 800.0f } } },
	{ "Panzer cam",			true,	"pcam",		CAT_SPYCAM, false,	{ { "Off", false }, { "On", true } } },
	{ "Mirror cam",			false,	"mircam",	CAT_SPYCAM, false,	{ { "Off", false }, { "On", true } } },

};

// save all configurations to a file
void CNexMenu::writeConfig() {
	FILE *file;
	
	if ((file = fopen(sNex.cfgFilename, "w")) == NULL) {
		#ifdef NEX_LOGGING
			nexUtils.log("ERROR [MENU]: can't write config file");
		#endif
		return;
	}
	
	int count = 0;
	for (; count < VARS_TOTAL; count++) {
		if (!nexVars[count].cvarName) {
			#ifdef NEX_LOGGING
				nexUtils.log("ERROR [MENU]: VAR_%i undefined", count);
			#endif
		} else if (nexValue[count] == (float)(int)nexValue[count]) {
			fprintf(file, "%s=%i\n", nexVars[count].cvarName, (int)nexValue[count]);
		} else {
			fprintf(file, "%s=%.2f\n", nexVars[count].cvarName, nexValue[count]);
		}
	}

	fclose(file);
}

// get vars from a cfg file
void CNexMenu::readConfig() {
	FILE *file;

	// Init all user vars with the default value
	int count = 0;
	for (; count < VARS_TOTAL; count++)
		nexValue[count] = nexVars[count].defaultValue;

	if ((file = fopen(sNex.cfgFilename, "rb")) == NULL)
		return;

	// Get config file line by line
	char line[32];
	while (fgets(line, sizeof(line) - 1, file) != 0) {
		char *sep = strrchr(line, '=');
		*sep = '\0';	// Separate name from value
	
		// Search for this var
		int count = 0;
		for (; count < VARS_TOTAL; count++) {
			if (!nexVars[count].cvarName) {
				#ifdef NEX_LOGGING
					nexUtils.log("ERROR [READCONFIG]: VAR_%i undefined", count);
				#endif
				continue;
			} else if (!strcmp(line, nexVars[count].cvarName)) {
				nexValue[count] = atof(sep + 1);
				break;
			} else if ((count + 1) == VARS_TOTAL) {
				#ifdef NEX_LOGGING
					nexUtils.log("WARNING [READCONFIG]: unknown var: [%s]", line);
				#endif
				continue;
			}
		}
	}

	fclose(file);
}

// draw menu ~_~
void CNexMenu::drawMenu() {
	// delay between 2 clicks
	bool doClick = false;
	static int lastClick = 0;

	if ((GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON) || GetAsyncKeyState(VK_MBUTTON)))
		doClick = ((nex.cg_time - lastClick) > NEX_MENU_KEY_DELAY);

	// Get mouse/key events
	orig_syscall(CG_KEY_SETCATCHER, KEYCATCH_UI | orig_syscall(CG_KEY_GETCATCHER));

	// Get item height
	int itemHeight = nexDraw.sizeTextHeight(MENU_TEXT_SIZE, &nex.media.mainFont) + (2 * 2);

	// Get max menu text width size
	int maxItemWidth = 0;
	int count;
	for (count = 0; count < VARS_TOTAL; count++) {
		int size = nexDraw.sizeTextWidth(MENU_TEXT_SIZE, nexVars[count].menuText, &nex.media.mainFont);
		if (size > maxItemWidth)
			maxItemWidth = size;
	}
	maxItemWidth += itemHeight;

	// Get max choice text width size
	int maxValueWidth = 0;
	for (count = 0; count < VARS_TOTAL; count++) {
		int i = 0;
		for (; i < MAX_CHOICES; i++) {
			int size = nexDraw.sizeTextWidth(MENU_TEXT_SIZE, nexVars[count].choices[i].name, &nex.media.mainFont);
			if (size > maxValueWidth)
				maxValueWidth = size;
		}
	}
	maxValueWidth += itemHeight;
	
	static int openCategory = -1;
	int nextOpenCategory = openCategory;
	int y = MENU_SCREEN_BORDER;
	int catCount = 0;
	for (; catCount < CATS_MAX; catCount++) {
		bool isMouseOn = ((nex.mouse[0] > MENU_SCREEN_BORDER)
				&& (nex.mouse[0] < (MENU_SCREEN_BORDER + maxItemWidth))
				&& (nex.mouse[1] > y)
				&& (nex.mouse[1] < (y + itemHeight)));

		// Sanity check
		if (!menuCats[catCount].name) {
			#ifdef NEX_LOGGING
				nexUtils.log("FATAL ERROR [DRAW MENU]: Can't find text for a menu category: CAT_%i", catCount);
			#endif
			orig_syscall(CG_ERROR, "Can't find text for a menu category");
		}

		// Draw category
		nexDraw.drawFillRect(MENU_SCREEN_BORDER, y, maxItemWidth, itemHeight, isMouseOn ? colorBlack : gameBackColor);
		nexDraw.drawRect(MENU_SCREEN_BORDER, y, maxItemWidth, itemHeight, isMouseOn ? gameBackColor : gameBorderColor, 1);
		nexDraw.drawText(MENU_SCREEN_BORDER + (itemHeight / 2), y + 2, MENU_TEXT_SIZE, menuCats[catCount].name, isMouseOn ? gameFontColor : colorWhite, qfalse, qfalse, &nex.media.mainFont);

		y += itemHeight + MENU_ITEM_SPACE;

		// Category click
		if ((GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON) || GetAsyncKeyState(VK_MBUTTON)) && isMouseOn && doClick) {
			lastClick = nex.cg_time;
			if (nextOpenCategory != catCount)
				nexSyscall.S_StartLocalSound(nex.media.limboSelect, CHAN_LOCAL_SOUND);
			nextOpenCategory = catCount;
		}

		// Draw vars only for this category
		if (openCategory != catCount)
			continue;

		// Search vars in this category
		int varCount = 0;
		for (; varCount < VARS_TOTAL; varCount++) {
			if (menuCats[catCount].id != nexVars[varCount].menuCategory)
				continue;

			nexVars_t *var = &nexVars[varCount];
			isMouseOn = ((nex.mouse[0] > MENU_SCREEN_BORDER)
					&& (nex.mouse[0] < (MENU_SCREEN_BORDER + maxItemWidth + MENU_ITEM_SPACE + maxValueWidth))
					&& (nex.mouse[1] > y)
					&& (nex.mouse[1] < (y + itemHeight)));

			// Draw var
			nexDraw.drawFillRect(MENU_SCREEN_BORDER, y, maxItemWidth, itemHeight, isMouseOn ? colorBlack : gameBackColor);
			nexDraw.drawRect(MENU_SCREEN_BORDER, y, maxItemWidth, itemHeight, isMouseOn ? gameBackColor : gameBorderColor, 1);
			nexDraw.drawText(MENU_SCREEN_BORDER + (itemHeight / 2) + nexDraw.sizeTextWidth(MENU_TEXT_SIZE, " ", &nex.media.mainFont), y + 2, MENU_TEXT_SIZE, var->menuText, colorLtGrey, qfalse, qfalse, &nex.media.mainFont);

			// Search position of the current value
			int pos = 0;
			while ((var->choices[pos].name != NULL) // Check for valid choice
					&& (var->choices[pos].value != nexValue[varCount])
					&& (pos < MAX_CHOICES)) {
				pos++;
			}

			// Get name of this value
			char value[32];
			memset(value, 0, sizeof(value));
			if ((pos == MAX_CHOICES) || (var->choices[pos].name == NULL))
				sprintf(value, "%.2f", nexValue[varCount]);
			else
				strncpy(value, var->choices[pos].name, sizeof(value));

			// Draw value of this var
			nexDraw.drawFillRect(MENU_SCREEN_BORDER + MENU_ITEM_SPACE + maxItemWidth, y, maxValueWidth, itemHeight, isMouseOn ? colorBlack : gameBackColor);
			nexDraw.drawRect(MENU_SCREEN_BORDER + MENU_ITEM_SPACE + maxItemWidth, y, maxValueWidth, itemHeight, isMouseOn ? gameBackColor : gameBorderColor, 1);
			int w = nexDraw.sizeTextWidth(MENU_TEXT_SIZE, value, &nex.media.mainFont);
			nexDraw.drawText(MENU_SCREEN_BORDER + MENU_ITEM_SPACE + maxItemWidth + (maxValueWidth / 2) - (w / 2), y + 2, MENU_TEXT_SIZE, value, colorLtGrey, qfalse, qfalse, &nex.media.mainFont);
			
			y += itemHeight + MENU_ITEM_SPACE;

			// If not a value click
			if ((!GetAsyncKeyState(VK_LBUTTON) && !GetAsyncKeyState(VK_RBUTTON)) || !isMouseOn || !doClick)
				continue;

			lastClick = nex.cg_time;
				
			// Get max choices
			int max = 0;
			while (var->choices[max].name != NULL)
				max++;
				
			// Pre-define values
			if (max) {
				// If current value no found take 0
				if ((pos < 0) || (pos == MAX_CHOICES))
					pos = 0;

				// Set new position
				if (GetAsyncKeyState(VK_LBUTTON))
					pos++;
				else
					pos--;

				// Check bounds
				if (pos >= max)
					pos = 0;
				else if (pos < 0)
					pos = max - 1;

			    nexValue[varCount] = var->choices[pos].value;
			// Manual value
			} else {
				// Set new value
				if (GetAsyncKeyState(VK_LBUTTON)) {
					nexValue[varCount] += nexValue[VAR_STEP];
					if ((varCount == VAR_AIMFOV) && (nexValue[varCount] > 360.0f))
						nexValue[varCount] -= 360.0f;
				}
				else {
					nexValue[varCount] -= nexValue[VAR_STEP];
					if ((varCount == VAR_AIMFOV) && (nexValue[varCount] < 0.01f)) // 0.01 to make it 360.0 instead of 0.0 when decreasing
						nexValue[varCount] += 360.0f;
				}
			}

			nexSyscall.S_StartLocalSound(nex.media.limboSelect, CHAN_LOCAL_SOUND);
		}
	}
	openCategory = nextOpenCategory;
}
