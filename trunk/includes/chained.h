/*
** Chained in Sanity
** Core library include
** Try to keep this neat please!
**/

#ifndef _INCLUDES_CHAINED_H_
#define _INCLUDES_CHAINED_H_

#define true 1
#define false 0

/*
** These files are required by pretty much every part of chained ... 
** Hence why we have them here :P
**/

#include <stdio.h>
/*#include <stdlib.h>*/
#include <errno.h> /* helpful ;) */

#include "config.h"

/*
 * Lets have some types...
 */

typedef char bool;
#include <types/connection.h>


#include "main.h"

#endif