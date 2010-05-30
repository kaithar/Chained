/**
 * @file main.c
 * @brief Basic abstracted connection methods.
 * Chained in Sanity.
 * Core Library.
 * This file contains methods that are designed to handle common basic tasks relating to connections.
 * These method's should work happily on any connection type.
 */

#include "libchained/chained.h"
#include <stdarg.h>  /* va_*() */

/**
 * This buffer is used as a temp read in.
 * It's 10k big ... if you try and accept lines bigger than that, I will have to hurt you.
 */
char conn_read_in_temp[10000];

/**
 * I read new line delimited data from the connection and add it to the sendq.
 * This function relys on the connection allocating enough buffer space for the largest line it could expect to receive.
 * If the connection tries to send too bigger line, it gets hard wrapped.
 * It's rather important to note that this function prefers efficent memory use over efficent cpu usage.
 * This means that if we expect a max line of 10k, we use a 10k temp buffer and then malloc and memcpy.
 * This is in contrast to doing a malloc of 10k every time.
 *
 * We're allocating a block for each line...
 * I'm far too lazy to try and keep track of multiple messages in the same block of memory.
 * Honest.
 */
void conn_read_to_recvq (connection *cn)
{
  int nbytes = 0;

  char *next = NULL;
  int foo = 0;

  while (1)
  {
    if (cn->recvq->queue_size > 10000)
    {
      /* Um, that's a lot of data ... lets just let that empty a little .... */
      break;
    }

    if (((nbytes = cn->read(cn)) <= 0) || (cn->state.local_read_shutdown == 1) || (cn->state.remote_closed == 1))
    {
      /* We expect cn->read to handle it's own errors and return 0 when we need to stop reading. */
      break;
    }
  }
}

bool conn_send_from_sendq (connection *cn)
{
  /**
  * If there is no sendq, return true in case differenciation is needed.
  */
  if (cn->sendq->queue_size == 0)
  {
    socketengine->mod(cn,0,-1);
    return 1;
  }

  /**
  * If the connection is closed, attempt to empty the sendq.
  * Dead connections can recv messages though...
  */

  if (cn->state.remote_closed == 1)
  {
    buffer_empty(cn->sendq);
    socketengine->mod(cn,0,-1);
    return 1;
  }

  if (cn->write(cn) < 0)
    return 0;

  if (cn->sendq->queue_size == 0)
  {
    socketengine->mod(cn,0,-1);
  }
  return 1;
}

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
  socketengine->mod(stream,0,1);

  return nbytes;
}
