/*
 * lib.h
 *
 *  Created on: 2013/09/14
 *      Author: Turenar (snswinhaiku dot lo at gmail dot com)
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <tchar.h>

#ifndef PATH_SEPARATOR
#define PATH_SEPARATOR _T("\\")
#endif /* PATH_SEPARATOR */

#ifndef PATH_SEPARATOR_LEN
#define PATH_SEPARATOR_LEN 1
#endif /* PATH_SEPARATOR_LEN */

LPTSTR path_combine(LPCTSTR dir, LPCTSTR file) __attribute__((malloc));

#ifdef HAVE_REALPATH
#define __fullpath(fpath,resolved,size) realpath((fpath), (resolved))
#else
#define __fullpath(fpath,resolved,size) _tfullpath((resolved), (fpath), (size))
#endif /* realpath */

#ifndef HAVE_TDIRNAME
#define _tdirname(path) __tdirname((path))
LPTSTR __tdirname(LPCTSTR path) __attribute__((malloc));
#endif /* dirname */

#ifndef _tbuflen
#define _tbuflen(buf) (sizeof((buf))/sizeof(TCHAR))
#endif

#endif /* COMMON_H_ */
