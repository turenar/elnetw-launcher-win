/*
 * common.c
 *
 *  Created on: 2013/09/14
 *      Author: Turenar (snswinhaiku dot lo at gmail dot com)
 */
#include "config.inc.h"

#include <string.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>

#include "api.h"
#include "common.h"

LPTSTR path_combine(LPCTSTR dir, LPCTSTR file) {
	int len;
	len = _tcslen(dir);
	len += _tcslen(file);
	len += PATH_SEPARATOR_LEN + 1; // NULL char

	LPTSTR buf = (LPTSTR) calloc(len, sizeof(TCHAR));
	(*buf) = 0;

	_tcscat(buf, dir);
	_tcscat(buf, PATH_SEPARATOR);
	_tcscat(buf, file);

	return buf;
}

#ifndef HAVE_TDIRNAME
LPTSTR __tdirname(LPCTSTR path) {
	int i = _tcslen(path);
	for (; i >= 0; i--) {
		if (path[i] == _T('\\'))
			break;
	}
	LPTSTR ret = (LPTSTR) malloc((i + 1) * sizeof(TCHAR));
	_tcsncpy(ret, path, i);
	ret[i] = _T('\0');
	return ret;
}
#endif /* dirname */

void report_error(DWORD result) {
	LPVOID lpMsgBuf;
	FormatMessage(
	FORMAT_MESSAGE_ALLOCATE_BUFFER |
	FORMAT_MESSAGE_FROM_SYSTEM |
	FORMAT_MESSAGE_IGNORE_INSERTS,
	NULL, result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 既定の言語
	(LPTSTR) &lpMsgBuf, 0,
	NULL);

	MessageBox(NULL, (LPCTSTR) lpMsgBuf, _T("Error"),
	MB_OK | MB_ICONINFORMATION);
}

LPTSTR get_registry_str(HKEY root, LPCTSTR dir, LPCTSTR path) {
	HKEY hKey;
	DWORD regType;
	DWORD regSize;
	DWORD result;
	result = RegOpenKeyEx(root, dir, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
	if (result != ERROR_SUCCESS) {
		report_error(result);
	}
	result = RegQueryValueEx(hKey, path, NULL, &regType, NULL, &regSize);
	if (result != ERROR_SUCCESS || regType != REG_SZ) {
		report_error(result);
		return NULL;
	}
	LPTSTR data = malloc(regSize);
	result = RegQueryValueEx(hKey, path, NULL, &regType, (LPBYTE) data,
			&regSize);
	if (result != ERROR_SUCCESS || regType != REG_SZ) {
		report_error(result);
		return NULL;
	}
	return data;
}

static PVOID wowRedirectOldValue;
void wow_redirect_start() {
	Wow64DisableWow64FsRedirection(&wowRedirectOldValue);
}

void wow_redirect_stop() {
	Wow64RevertWow64FsRedirection(wowRedirectOldValue);
}

int proc_spawn(LPTSTR execPath, LPTSTR* cmdLine) {
	PROCESS_INFORMATION processInformation = { 0 };
	STARTUPINFO startupInfo = { 0 };
	startupInfo.cb = sizeof(startupInfo);

	int size = 0;
	LPTSTR* cmdLine_lp = cmdLine;
	while ((*cmdLine_lp) != NULL) {
		size += _tcslen(*(cmdLine_lp++)) + 3; // '"" '
	}
	// last space is \0, so do not touch size
	LPTSTR cmdLineStr = malloc((size + 1) * sizeof(TCHAR));
	cmdLineStr[0] = _T('\0');

	cmdLine_lp = cmdLine;
	while ((*cmdLine_lp) != NULL) {
		_tcscat(cmdLineStr, _T("\""));
		_tcscat(cmdLineStr, *(cmdLine_lp++));
		_tcscat(cmdLineStr, _T("\" "));
	}

	// Create the process
	BOOL result = CreateProcess(execPath, cmdLineStr,
	NULL, NULL, FALSE,
	NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
	NULL, NULL, &startupInfo, &processInformation);

	if (!result) {
		// CreateProcess() failed
		// Get the error from the system
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();
		FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
						| FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf, 0, NULL);

		// Display the error
		LPTSTR strError = (LPTSTR) lpMsgBuf;
		MessageBox(NULL, strError, _T("elnetw"), MB_OK | MB_ICONERROR);

		// Free resources created by the system
		LocalFree(lpMsgBuf);

		// We failed.
		return -1;
	} else {
		// Successfully created the process.  Wait for it to finish.
		WaitForSingleObject(processInformation.hProcess, INFINITE);

		DWORD exitCode;
		// Get the exit code.
		result = GetExitCodeProcess(processInformation.hProcess, &exitCode);

		// Close the handles.
		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);

		return result ? exitCode : -1;
	}
}
