/*
 * api.h
 *
 *  Created on: 2014/10/01
 *      Author: Turenar (snswinhaiku dot lo at gmail dot com)
 */

#ifndef API_H_
#define API_H_

#include "config.inc.h"
#include <windows.h>

#ifndef HAVE_WOW64DISABLEWOW64FSREDIRECTION
BOOL WINAPI Wow64DisableWow64FsRedirection(PVOID *OldValue);
BOOL WINAPI Wow64RevertWow64FsRedirection(PVOID OldValue);
#endif

#endif /* API_H_ */
