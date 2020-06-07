// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

CNexSdk nexSdk;

void CNexSdk::CG_ParseReinforcementTimes(const char *pszReinfSeedString) {
	const char *tmp = pszReinfSeedString, *tmp2;
	unsigned int i, j, dwDummy, dwOffset[TEAM_NUM_TEAMS];

	#define GETVAL(x,y) if((tmp = strchr(tmp, ' ')) == NULL) return; x = atoi(++tmp)/y;

	dwOffset[TEAM_ALLIES] = atoi(pszReinfSeedString) >> REINF_BLUEDELT;
	GETVAL(dwOffset[TEAM_AXIS], (1 << REINF_REDDELT));
	tmp2 = tmp;

	for(i=TEAM_AXIS; i<=TEAM_ALLIES; i++) {
		tmp = tmp2;
		for(j=0; j<MAX_REINFSEEDS; j++) {
			if(j == dwOffset[i]) {
				GETVAL(nex.cgs_aReinfOffset[i], aReinfSeeds[j]);
				nex.cgs_aReinfOffset[i] *= 1000;
				break;
			}
			GETVAL(dwDummy, 1);
		}
	}
}

