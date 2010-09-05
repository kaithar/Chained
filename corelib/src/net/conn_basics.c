/**
 * @file main.c
 * @brief Basic abstracted connection methods.
 * Chained in Sanity.
 * Core Library.
 * This file contains methods that are designed to handle common basic tasks relating to connections.
 * These method's should work happily on any connection type.
 */

#include "includes/chained.h"
#include <stdarg.h>  /* va_*() */

/* Yeah, this is a plain and simple wrapper.
 * At some point I'll trial this against a custom printf variation, but for now it'll do.
 * (cause stuff like channel support is more important)
 */

static char sharbuf[10000];

int cprintf(connection *stream, char *fmt, ...)
{
  va_list argv;
  int nbytes = 0;
  char *temp;

  va_start(argv, fmt);
  nbytes = vsnprintf(sharbuf, 9999, fmt, argv);
  va_end(argv);

  if (nbytes > 9999)
  {
    sharbuf[9999] = '\0';
    nbytes = 9999;
  }

  if (nbytes < 0)
    return nbytes;

  /* Try and send it... */
  buffer_store(stream->sendq, sharbuf, nbytes);
  r_config.socketengine->mod(stream,0,1);

  return nbytes;
}
