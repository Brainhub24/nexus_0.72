// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include "nexus.h"

CNexUtils nexUtils;

void CNexUtils::init(HINSTANCE hInst) {
	// reset our structs
	resetNex();
	memset(&sNex, 0, sizeof(sNex_t));

	int len = GetModuleFileNameA(hInst, sNex.path, sizeof(sNex.path));
	if (len) {
		while (len && sNex.path[len] != '\\')
			len--;
		if (len)
			sNex.path[len + 1] = '\0';

		// logging
		#ifdef NEX_LOGGING
			fopen_s(&sNex.logFile, va("%s%s", sNex.path, NEX_LOGFILE), "w");
			log("NEXUS loaded!");
			log("Running version [" NEX_VERSION " " NEX_EDITION "]");
		#endif

		sNex.etSupported = detectET();

		getSettingsFromConf();
	}

	if (sNex.etSupported) {
		orig_syscall = (syscall_t) sNex.ET->syscall;
		nexHook.hookSY(true);
		nexHook.hookET(true);
	}
}

void CNexUtils::getSettingsFromConf() {
	char *iniConf = va("%s%s", sNex.path, NEX_INI);
	CIniReader config(iniConf);
	strncpy(sNex.prefix, config.ReadString("Nexus", "prefix", NEX_PREFIX), sizeof(sNex.prefix));
	sprintf_s(sNex.cfgFilename, sizeof(sNex.cfgFilename), "%s%s", sNex.path, config.ReadString("Nexus", "config", NEX_CONFIG));
	strncpy(sNex.etproGuid, config.ReadString("Nexus", "guid", NEX_EG), sizeof(sNex.etproGuid));
	sNex.etproSpoof = config.ReadBoolean("Nexus", "spoof", NEX_ES);
	strncpy(sNex.aimKey, config.ReadString("Nexus", "aimkey", NEX_AIMKEY), sizeof(sNex.aimKey));
	strncpy(sNex.ksFormat, config.ReadString("Nexus", "ksformat", NEX_KS), sizeof(sNex.ksFormat));
	strncpy(sNex.pk3Name, config.ReadString("Nexus", "pk3", NEX_PK3), sizeof(sNex.pk3Name));
	#ifdef NEX_VERBOSE
		log("Cmd preifx: %s", sNex.prefix);
		log("Config: %s", sNex.cfgFilename);
		log("ETpro spoofing: %i", sNex.etproSpoof);
		log("ETpro guid: %s", sNex.etproGuid);
		log("Killspam format: %s", sNex.ksFormat);
		log("pk3 name: %s", sNex.pk3Name); 
	#endif

	nexMenu.readConfig();
	nexMenu.writeConfig();
}

void CNexUtils::resetNex() {
	memset(&nex, 0, sizeof(nex_t));
	nex.cg_clientNum = -1;
}

#ifdef NEX_LOGGING
	void CNexUtils::log(const char *format, ...) {
		if (sNex.logFile) {
			char buffer[4096];
			va_list va;
			va_start(va, format);
				vsprintf_s(buffer, sizeof(buffer), format, va);
			va_end(va);

			fprintf_s(sNex.logFile, "%s\n", buffer);
		}
	}
#endif

bool CNexUtils::detectET() {
	sNex.etHandle = GetModuleHandleA("ET.exe");

	if (sNex.etHandle) {
		int len = GetModuleFileNameA(sNex.etHandle, sNex.etPath, sizeof(sNex.etPath));
		if (len) {
			u_long crc32 = crc32FromFile(sNex.etPath);
			#ifdef NEX_LOGGING
				log("ET.exe crc32: 0x%x", crc32);
			#endif
			
			while (len && sNex.etPath[len] != '\\')
				len--;
			if (len)
				sNex.etPath[len + 1] = '\0';

			static int numVersion = sizeof(nexETs) / sizeof(nexET_t);

			int i = 0;
			for (; i < numVersion; i++) {
				if (crc32 == nexETs[i].crc32) {
					sNex.ET = &nexETs[i];

				#ifdef NEX_LOGGING
						log("Found ET version %s", sNex.ET->version);
				#endif

				return true;
				}
			}
		}
	}

	#ifdef NEX_LOGGING
		log("This ET version isn't supported!");
	#endif
	return false;
}

bool CNexUtils::detectModLib(const char *dll) {
	u_long crc32 = crc32FromFile(dll);

	#ifdef NEX_LOGGING
		log("cgame crc32: 0x%x", crc32);
	#endif

	static int numMods = sizeof(nexMods) / sizeof(nexMod_t);

	for (int i=0; i < numMods; i++) {
		if (crc32 == nexMods[i].crc32) {
			nex.mod = &nexMods[i];
			#ifdef NEX_LOGGING
				log("Detected mod: %s %s", nex.mod->name, nex.mod->version);
			#endif
			return true;
		}
	}

	return false;
}

unsigned long CNexUtils::crc32FromFile(const char* filename) {
	FILE *file;
	
	unsigned long crc32;
	int err;

	if ((file = fopen(filename, "rb")) == NULL)
		return 0;

	err = Crc32_ComputeFile(file, &crc32);

	if (err == -1)
		return 0;

	return crc32;
}

char *CNexUtils::strndup(const char *s, size_t n) {
	// http://www.koders.com/c/fid1867004532ABB00D9750ACC47A1366D52E8C616C.aspx
	size_t len = strlen(s);
	char *neww = (char *)malloc (len + 1);

	if (neww == NULL)
	return NULL;

	neww[len] = '\0';
		return (char *) memcpy(neww, s, len);
}
