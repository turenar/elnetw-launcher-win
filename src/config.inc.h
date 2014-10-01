/*
 * config.inc.h
 *
 *  Created on: 2014/10/01
 *      Author: Turenar (snswinhaiku dot lo at gmail dot com)
 */

#ifndef CONFIG_INC_H_
#define CONFIG_INC_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <windef.h>

#ifndef __ATTRIB_MALLOC__
#define __ATTRIB_MALLOC__ __attribute__((__malloc__))
#endif /* __ATTRIB_MALLOC__ */

#ifndef NULL
#define NULL ((void*) 0)
#endif /* ifndef NULL */

#endif /* CONFIG_INC_H_ */
