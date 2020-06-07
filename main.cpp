// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

// structs
sNex_t sNex;
nex_t nex;

bool WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID pvReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		#ifdef NEX_VERBOSE
			nexUtils.log("DllMain: DLL_PROCESS_ATTACH");
		#endif

		nexUtils.init(hInst);
	}

	return true;
}
