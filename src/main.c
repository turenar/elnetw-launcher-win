/*
 * main.c
 *
 *  Created on: 2013/09/14
 *      Author: Turenar (snswinhaiku dot lo at gmail dot com)
 */

#include "config.inc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>
#include <locale.h>
#include <tchar.h>
#include <process.h>

#include "node.h"
#include "arg_parser.h"
#include "common.h"

static TCHAR cwd[MAX_PATH];
static LPTSTR project_dir;
static LPTSTR bin_dir;
static llist app_args;
static llist java_args;
static LPTSTR launcher_jar = NULL;

void add_app_arg(LPTSTR arg) {
	node_add_list(&app_args, arg);
}

void add_java_arg(LPTSTR arg) {
	node_add_list(&java_args, arg);
}

LPTSTR get_arg_value(LPTSTR haystack, LPCTSTR needle) {
	LPTSTR pointer = haystack;
	for (; needle != _T('\0');) {
		if ((pointer++) != (needle++)) {
			return NULL;
		}
	}
	return pointer;
}

void process_args(int argc, LPTSTR *argv) {
	int i;
	for (i = 1; i < argc; i++) {
		LPTSTR arg = argv[i];
		if (arg[0] == _T('-') && arg[1] == _T('X')) {
			LPTSTR val = get_arg_value(arg, _T("-Xlauncher.jar="));
			if (val != NULL) {
				launcher_jar = val;
			}
		} else {
			add_app_arg(arg);
		}
	}
}

bool check_if_exists(LPCTSTR path) {
	struct __stat64 st;

	int ret = _tstat64(path, &st);
	return !ret;
}

LPTSTR get_launcher_jar() {
	if (launcher_jar != NULL)
		return launcher_jar;

	LPTSTR launcher_jar = path_combine(bin_dir, _T("launcher.jar"));
	LPTSTR jar_name;

	if (check_if_exists(launcher_jar)) {
		jar_name = launcher_jar;
	} else {
		free(launcher_jar);

		LPTSTR dist_jar = path_combine(project_dir,
		_T("target") PATH_SEPARATOR _T("elnetw-dist.jar"));
		if (check_if_exists(dist_jar)) {
			jar_name = dist_jar;
			add_java_arg(_T("-Dconfig.portable=true"));
		} else {
			MessageBox(NULL,
					_T("エラー: elnetwの起動に失敗しました\nbin/launcher.jarまたはtarget/elnetw-dist.jarが見つかりません。"
					), _T("elnetw"), MB_OK | MB_ICONERROR);
			exit(1);
		}
	}
	return jar_name;
}

int _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR pCmdLine,
		int showCmd) {
	TCHAR fullpath[MAX_PATH];
	int argc;
	pCmdLine = GetCommandLine();
	LPTSTR *argv = parse_args(pCmdLine, &argc);

	__fullpath(argv[0], fullpath, MAX_PATH);
	bin_dir = _tdirname(fullpath);

	_tgetcwd(cwd, _tbuflen(cwd));
	if (_tcscmp(bin_dir, cwd)) {
		_tchdir(_T(".."));
	}
	project_dir = _tcsdup(bin_dir);
	project_dir = _tdirname(project_dir);

	process_args(argc, argv);

	launcher_jar = get_launcher_jar();
	add_app_arg(_T("-L"));
	add_app_arg(path_combine(project_dir, _T("lib")));

	llist commands = { 0 };
	node_add_list(&commands, _T("java"));
	node_add_all(&commands, &java_args);
	node_add_list(&commands, _T("-jar"));
	node_add_list(&commands, launcher_jar);
	node_add_all(&commands, &app_args);

	TCHAR str[65536] = { _T('\0') };
	struct _link_node *j = commands.first;
	while (j != NULL) {
		_tcscat(str, _T("\""));
		_tcscat(str, j->obj);
		_tcscat(str, _T("\" "));
		j = j->next;
	}

	//MessageBox(NULL, str, _T("elnetw"), MB_OK);

	int exitCode = _tspawnvp(_P_WAIT, _T("java"),
			(LPCTSTR const*) node_to_array(&commands));


	switch (exitCode) {
	case -1:
		MessageBox(NULL,
				_T("起動に失敗しました。javaコマンドが見つかりません。\nJava Runtime Environmentがインストールされているか確認して下さい"),
				_T("elnetw"), MB_OK | MB_ICONERROR);
		break;
	case 1:
		break;
		//MessageBox(NULL, _T("エラーコード1"), _T("elnetw"), MB_OK);
	}

	return exitCode;
}

#ifdef _UNICODE
#ifndef _tWinMain // Mingw32 does not implement wide startup module
int main(int argc, char **argv)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	int retval = 0;

	retval = _tWinMain(hInstance, NULL, _T(""), SW_SHOW);

	return retval;
}
#endif
#endif /* _UNICODE */

