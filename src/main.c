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
#include <unistd.h>

#include "node.h"
#include "arg_parser.h"
#include "common.h"

static TCHAR cwd[MAX_PATH];
static LPTSTR project_dir;
static LPTSTR bin_dir;
static llist app_args;
static llist java_args;
static llist classpath;

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
			/*LPTSTR val = get_arg_value(arg, _T("-Xlauncher.jar="));
			 if (val != NULL) {
			 launcher_jar = val;
			 }*/
		} else {
			add_app_arg(arg);
		}
	}
}

BOOL check_if_exists(LPCTSTR path) {
	struct __stat64 st;

	int ret = _tstat64(path, &st);
	return !ret;
}

void init_classpath() {
	LPTSTR launcher_jar = path_combine(bin_dir, _T("launcher.jar"));

	if (check_if_exists(launcher_jar)) {
		node_add_list(&classpath, launcher_jar);
		node_add_list(&classpath, path_combine(bin_dir, _T("library.jar")));
	} else {
		free(launcher_jar);

		LPTSTR dist_jar = path_combine(project_dir,
		_T("target") PATH_SEPARATOR _T("elnetw-dist.jar"));
		if (check_if_exists(dist_jar)) {
			node_add_list(&classpath, dist_jar);
			add_java_arg(_T("-Dconfig.portable=true"));
		} else {
			MessageBox(NULL, _T("エラー: elnetwの起動に失敗しました\n"
					"bin/launcher.jarまたはtarget/elnetw-dist.jarが見つかりません。"),
					_T("elnetw"), MB_OK | MB_ICONERROR);
			exit(1);
		}
	}
}

LPTSTR get_java_cmd() {
	LPTSTR regValue =
			get_registry_str(HKEY_LOCAL_MACHINE,
					_T("SOFTWARE\\JavaSoft\\Java Runtime Environment"),
					_T("CurrentVersion"));
#ifdef DEBUG
	MessageBox(NULL, regValue, _T("elnetw"), MB_OK);
#endif
	if (regValue != NULL) {
		LPTSTR regVersionKey = path_combine(
				_T("SOFTWARE\\JavaSoft\\Java Runtime Environment"), regValue);
#ifdef DEBUG
		MessageBox(NULL, regVersionKey, _T("elnetw"), MB_OK);
#endif
		LPTSTR regValue = get_registry_str(HKEY_LOCAL_MACHINE, regVersionKey, _T("JavaHome"));
		if (regValue != NULL) {
			LPTSTR javaPath = path_combine(regValue, _T("bin\\javaw.exe"));
			free(regValue);
			return javaPath;
		}
	}
	if (check_if_exists(_T("C:\\Windows\\System32\\javaw.exe"))) {
		return _T("C:\\Windows\\System32\\javaw.exe");
	} else {
		return NULL;
	}
}

int el_main(){
	TCHAR fullpath[MAX_PATH];
	int argc;
	LPTSTR pCmdLine = GetCommandLine();
	LPTSTR *argv = parse_args(pCmdLine, &argc);

	__fullpath(argv[0], fullpath, MAX_PATH);
	bin_dir = _tdirname(fullpath);

	_tgetcwd(cwd, _tbuflen(cwd));
	if (_tcscmp(bin_dir, cwd) == 0) {
		_tchdir(_T(".."));
	}
	project_dir = _tdirname(bin_dir);

	process_args(argc, argv);

	init_classpath();
	add_app_arg(_T("-L"));
	add_app_arg(path_combine(project_dir, _T("lib")));

	llist commands = { 0 };
	LPTSTR java_cmd = get_java_cmd();
	if (java_cmd == NULL) {
		MessageBox(NULL, _T("javaコマンドが見つかりません！"), _T("elnetw"),
		MB_OK | MB_TASKMODAL | MB_TOPMOST);
	}
	node_add_list(&commands, java_cmd);
	node_add_all(&commands, &java_args);
	node_add_list(&commands, _T("-classpath"));
	node_add_list(&commands, node_combine_str(&classpath, _T(";")));
	node_add_list(&commands, _T("jp.mydns.turenar.launcher.TwitterClientLauncher"));
	node_add_all(&commands, &app_args);

#ifdef DEBUG
	TCHAR str[65536] = { _T('\0') };
	struct _link_node *j = commands.first;
	while (j != NULL) {
		_tcscat(str, _T("\""));
		_tcscat(str, j->obj);
		_tcscat(str, _T("\" "));
		j = j->next;
	}

	_tprintf(_T("%s\n"), str);
	fflush(stdout);

	MessageBox(NULL, str, _T("elnetw"), MB_OK | MB_TASKMODAL | MB_TOPMOST);
#endif

	wow_redirect_stop();
	int exitCode = proc_spawn(java_cmd, (LPCTSTR*) node_to_array(&commands));
	wow_redirect_start();

	switch (exitCode) {
	case -1:
		MessageBox(NULL,
				_T(
						"起動に失敗しました。javaコマンドが見つかりません。\nJava Runtime Environmentがインストールされているか確認して下さい"),
				_T("elnetw"), MB_OK | MB_ICONERROR | MB_TASKMODAL);
		break;
	case 1:
		break;
	}
	return exitCode;
}

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR ignore,
		int showCmd) {
	return el_main();
}

#ifdef UNICODE
int APIENTRY WinMain(HINSTANCE inst, HINSTANCE prevInst, LPSTR args,
		int showCmd) {
	return el_main();
}
#endif
