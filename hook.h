// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

#include "etpro.h"

// LoadLibraryA
typedef HINSTANCE		(__stdcall *LoadLibraryA_t) (LPCSTR lpLibName);
extern LoadLibraryA_t	orig_LoadLibraryA;
HINSTANCE __stdcall		nex_LoadLibraryA(LPCSTR lpLibName);

// vmMains
typedef int (__cdecl *vmMain_t) (int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11);
extern vmMain_t orig_CG_vmMain;
int __cdecl nex_CG_vmMain(int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11);

// dllEntry
typedef void (__cdecl *dllEntry_t)(int (*syscallptr)(int arg,... ));
extern dllEntry_t orig_CG_dllEntry;
extern dllEntry_t orig_UI_dllEntry;
void __cdecl nex_UI_dllEntry(int (*syscallptr)(int arg,... ));
void __cdecl nex_CG_dllEntry(int (*syscallptr)(int arg,... ));

// syscalls
typedef int (__cdecl *syscall_t)(int cmd, ...);
extern syscall_t orig_syscall;
int __cdecl nex_CG_syscall(int command, ...);
int __cdecl nex_UI_syscall(int command, ...);

// pk3 unlocker
extern char goodChecksumsPak[BIG_INFO_STRING];
typedef void (__cdecl *FS_PureServerSetLoadedPaks_t)(const char *pakSums, const char *pakNames);
extern FS_PureServerSetLoadedPaks_t orig_FS_PureServerSetLoadedPaks;
void __cdecl nex_FS_PureServerSetLoadedPaks(const char *pakSums, const char *pakNames);

// Cmd_AddCommand
typedef void (*xcommand_t) (void);
typedef void (__cdecl *Cmd_AddCommand_t)(const char *cmd_name, xcommand_t function);
extern Cmd_AddCommand_t orig_Cmd_AddCommand;
void __cdecl nex_Cmd_AddCommand(const char *cmd_name, xcommand_t function);

class CNexHook {
	public:
		void hookSY(bool state);			// hook system funcs
		void hookCG(bool state);			// hook cgame.dll funcs
		void hookET(bool state);			// hook et.exe
		pack_t *getPack(char *filename);	// find our pak
		void unreferenceBadPk3s();			// -_-
		void addCommands();
	private:
};

extern CNexHook nexHook;
