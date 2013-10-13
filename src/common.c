/*
 * lib.c
 *
 *  Created on: 2013/09/14
 *      Author: Turenar (snswinhaiku dot lo at gmail dot com)
 */
#include "config.inc.h"

#include <string.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>

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
	ret[i]=_T('\0');
	return ret;
}

#endif /* dirname */
