/**
 * @file chained.h
 * @brief Core include.
 * Chained in Sanity
 * Core library include
 * Try to keep this neat please!
 */

#ifndef _INCLUDES_CHAINED_H_
#define _INCLUDES_CHAINED_H_

#define true (bool) 1
#define false (bool) 0
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
#include <assert.h> /* All the better to hit you with a clue by four */

// #include "includes/config.h"

/**
 * @name These files are fairly selfcontained, but depended on by some types. 
 */
/*@{*/

#include <includes/fifo.h>
#include <includes/linklist.h>
#include <includes/map.h>
#include <includes/persistent_map.h>
#include <includes/buffer.h>

/*@}*/
/**
 * @name Lets have some types...
 */
/*@{*/

#include <includes/types/connection.h>
#include <includes/types/timers.h>
#include <includes/types/socket_engine.h>
// #include <includes/types/config_reading.h>
// #include <includes/types/modules.h>
#include <includes/types/reactor_config.h>

/*@}*/
/**
 * @name Now lets have some prototypes
 */
/*{*/

#include <includes/main.h>
#include <includes/conn_basics.h>
#include <includes/strings.h>
#include <includes/support.h>
#include <includes/socketengines.h>
#include <includes/tcp.h>
// #include <includes/config_reading.h>
#include <includes/events.h>
// #include <includes/modules.h>
// #include <includes/ssl.h>
#include <includes/timers.h>

#include <includes/externs.h>

/*@}*/

#endif
