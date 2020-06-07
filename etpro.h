// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

// anti cheat func
typedef void (__cdecl *etproAntiCheat_t)(void *a, void *b, void *c, int checksum, void *e, char *orig_guid);
extern etproAntiCheat_t orig_etproAntiCheat;
void __cdecl nex_etproAntiCheat(void *a, void *b, void *c, int checksum, void *e, char *orig_guid);

class CNexEtpro {
	public:
		void redirectSyscall();
	private:
};

extern CNexEtpro nexEtpro;


DWORD HideFromAC(PVOID);

