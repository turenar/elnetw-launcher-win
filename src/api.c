/*
 * api.c
 *
 *  Created on: 2014/10/01
 *      Author: Subaru
 */

#include "config.inc.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#ifndef HAVE_WOW64DISABLEWOW64FSREDIRECTION
static HMODULE kernel32handle;

#ifdef EDITOR_HACK
// Eclipse is foolish, formatter is broken with typedef stdcall
typedef WINBOOL (*_api_redirector)(PVOID *);
typedef WINBOOL (*_api_revertor)(PVOID);
#else
typedef WINBOOL (__stdcall (*_api_redirector))(PVOID *);
typedef WINBOOL (__stdcall (*_api_revertor))(PVOID);
#endif

static _api_redirector fsDisableFunction; /*Wow64DisableWow64FsRedirection*/
static _api_revertor fsRedirectFunction; /*Wow64RevertWow64FsRedirection*/
static void init_func_addr() {
	if (!fsDisableFunction || !fsRedirectFunction) {
		kernel32handle = GetModuleHandle(_T("kernel32.dll"));
		if (!kernel32handle) {
			_tprintf(
					_T(
							"kernel32.dll failed to load, failed to disable FS redirection.\n"));
			return;
		}
		fsDisableFunction = (_api_redirector) GetProcAddress(kernel32handle,
				"Wow64DisableWow64FsRedirection");
		fsRedirectFunction = (_api_revertor) GetProcAddress(kernel32handle,
				"Wow64RevertWow64FsRedirection");
		if (!fsDisableFunction || !fsRedirectFunction) {
			FreeLibrary(kernel32handle);
			_tprinf(
					_T(
							"Wow64DisableWow64FsRedirection or Wow64RevertWow64FsRedirection "
									"functions missing.\n"));
			return;
		}
	}
}
BOOL WINAPI Wow64DisableWow64FsRedirection(PVOID *OldValue) {
	return fsDisableFunction(OldValue);
}
BOOL WINAPI Wow64RevertWow64FsRedirection(PVOID OldValue) {
	return fsRedirectFunction(OldValue);
}
#endif
