// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

class CNexSpycams {
	public:
		void drawSpycam(float x, float y, float width, float height, refdef_t *refDef, qhandle_t enemyShader, qhandle_t friendShader, qboolean crosshair);
		void mirror();
		void mortar();
		void rifle();
		void panzer();
		void satchel();
	private:
};

extern CNexSpycams nexSpycam;
