// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

class CNexUtils {
	public:
		void init(HINSTANCE hInst);
		void resetNex();
		#ifdef NEX_LOGGING
			void log(const char *format, ...);
		#endif
		bool detectET();
		bool detectModLib(const char *dll);
		unsigned long crc32FromFile(const char* filename);
		void getSettingsFromConf();
		char *strndup(const char *s, size_t n); // http://www.koders.com/c/fid1867004532ABB00D9750ACC47A1366D52E8C616C.aspx
	private:
};

extern CNexUtils nexUtils;
