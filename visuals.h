// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

class CNexVisuals {
	public:
		void drawAdvert();
		void drawRefCount();
		void drawSpawnTimer();
		void addChams(refEntity_t* refEnt, bool visible);
		void addEspColors(int entityNum, bool visible);
		void setColors();
		bool isRadar(int player, int *screen, float scale);
		void drawRadar(int scale);
		void drawEspEntities();
	private:
};

extern CNexVisuals nexVisuals;

// colors
extern vec4_t gameBackColor;
extern vec4_t gameBorderColor;
extern vec4_t gameFontColor;
