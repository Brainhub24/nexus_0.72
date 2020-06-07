// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

class CNexAimbot {
	public:
		bool getVisibleModelBodyTag(const int player, vec3_t *origin);
		bool CG_GetTag(int clientNum, char *tagname, orientation_t *or);
		void doAutoShoot(bool shoot);
		void doAimbot();
		int findNearestTarget(int targetFlag);
		void aimAt(vec3_t target);
		void rifleAimAt(vec3_t target);
		void doVecZ(int player);
	private:
};

extern CNexAimbot nexAimbot;

void aimbotUpCommand(void);

