/**
 * @file chained.h
 * @brief Core include.
 * Chained in Sanity
 * Core library include
 * Try to keep this neat please!
 */

#ifndef _INCLUDES_CHAINED_H_
#define _INCLUDES_CHAINED_H_

#define true 1
#define false 0
typedef char bool;
#define __MAXFDS__ 1024

/*
 * These files are required by pretty much every part of chained ... 
 * Hence why we have them here :P
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> /* helpful ;) */

#include "config.h"

/**
 * @name These files are fairly selfcontained, but depended on by some types. 
 */
/*@{*/

#include <linklist.h>
#include <map.h>

/*@}*/
/**
 * @name Lets have some types...
 */
/*@{*/

#include <types/connection.h>
#include <types/timers.h>
#include <types/socket_engine.h>
#include <types/config_reading.h>
#include <types/modules.h>

/*@}*/
/**
 * @name Now lets have some prototypes
 */
/*{*/

#include <main.h>
#include <conn_basics.h>
#include <strings.h>
#include <support.h>
#include <socketengines.h>
#include <ipv4.h>
#include <config_reading.h>
#include <events.h>
#include <modules.h>

/*@}*/

#endif
