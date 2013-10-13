/*
 * config.h
 *
 *  Created on: 2013/09/14
 *      Author: Turenar (snswinhaiku dot lo at gmail dot com)
 */

#ifndef CONFIG_INC_H_
#define CONFIG_INC_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <process.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

#ifndef bool
typedef int bool;
#ifndef false
#define false 0
#endif /* false */
#ifndef true
#define true 1
#endif /* true */
#endif /* bool */

#ifndef _tspawnvp
#ifdef UNICODE
#define _tspawnvp _wspawnvp
#else
#define _tspawnvp _spawnvp
#endif /* def UNICODE */
#endif /* ndef _tspawnvp */

#ifndef __ATTRIB_MALLOC__
#define __ATTRIB_MALLOC__ __attribute__((__malloc__))
#endif /* __ATTRIB_MALLOC__ */

#ifndef NULL
#define NULL ((void*) 0)
#endif /* ifndef NULL */

#endif /* CONFIG_INC_H_ */
