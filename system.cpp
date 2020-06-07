// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

// Hook some system funcs in here (library related, memset, etc)

LoadLibraryA_t orig_LoadLibraryA;

HINSTANCE __stdcall nex_LoadLibraryA(LPCSTR lpLibName) {
	HMODULE hM = orig_LoadLibraryA(lpLibName);

	if (sNex.etSupported && strstr(lpLibName, "cgame_mp_x86.dll")) {
		sNex.cgHandle = hM;
		#ifdef NEX_LOGGING
			nexUtils.log("CG loaded at 0x%p", hM);
		#endif
		if (!nexUtils.detectModLib(lpLibName)) {
			#ifdef NEX_LOGGING
				nexUtils.log("Error[CG]: This mod is not supported");
			#endif
			nexHook.hookET(false);
		} else {
			nexHook.hookCG(true);
			if (nex.mod->type == MOD_ETPRO)
				sNex.syscallptr = (DWORD) sNex.cgHandle + nex.mod->syscallptr;
		}
	} else if (sNex.etSupported && strstr(lpLibName, "ui_mp_x86.dll")) {
		orig_UI_dllEntry = (dllEntry_t) DetourFunction((BYTE *) GetProcAddress(hM, "dllEntry"), (BYTE *) nex_UI_dllEntry);
		nexHook.addCommands();
	}

	return hM;
}

