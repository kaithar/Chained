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

#include "libchained/config.h"

/**
 * @name These files are fairly selfcontained, but depended on by some types. 
 */
/*@{*/

#include <libchained/fifo.h>
#include <libchained/linklist.h>
#include <libchained/map.h>
#include <libchained/persistent_map.h>

/*@}*/
/**
 * @name Lets have some types...
 */
/*@{*/

#include <libchained/types/connection.h>
#include <libchained/types/timers.h>
#include <libchained/types/socket_engine.h>
#include <libchained/types/config_reading.h>
#include <libchained/types/modules.h>

/*@}*/
/**
 * @name Now lets have some prototypes
 */
/*{*/

#include <libchained/main.h>
#include <libchained/conn_basics.h>
#include <libchained/strings.h>
#include <libchained/support.h>
#include <libchained/socketengines.h>
#include <libchained/ipv4.h>
#include <libchained/config_reading.h>
#include <libchained/events.h>
#include <libchained/modules.h>
#include <libchained/ssl.h>
#include <libchained/timers.h>

/*@}*/

#endif
