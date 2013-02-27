/**
 * @file buffer.c
 * @brief Represents packets of data.
 * Chained in Sanity.
 * Data structures.
 * Controls a buffer object.
 * Abstractions to queue to present as a continue data stream.
 */

#include "libchained/chained.h"
#include <string.h>

buffer_queue *new_buffer_queue (int block_size)
{
  buffer_queue *nb = smalloc(sizeof(buffer_queue));
  nb->block_size = block_size;
  nb->queue_size = 0;
  nb->queue = fifo_create();

  return nb;
}

/** Remember, if you're storing a zero terminated string,
  * the zero is still a byte and must be included.
  */
void buffer_store (buffer_queue *buf, char *block, int size)
{
  buffer_packet *bp;
  char *pos = block;
  int copy_chunk;
  buf->queue_size += size;
  while (size > 0)
  {
    bp = smalloc(sizeof(buffer_packet));
    bp->content = smalloc(buf->block_size);
    if (size > buf->block_size)
      copy_chunk = buf->block_size;
    else
      copy_chunk = size;
    memcpy(bp->content, pos, copy_chunk);
    bp->length = copy_chunk;
    bp->start = 0;
    bp->eof = 0;
    pos += copy_chunk;
    size -= copy_chunk;
    fifo_add(buf->queue, bp);
  }
}

void buffer_eof (buffer_queue *buf)
{
  buffer_packet *bp;
  bp = smalloc(sizeof(buffer_packet));
  bp->length = 0;
  bp->start = 0;
  bp->eof = 1;
  fifo_add(buf->queue, bp);
}

int buffer_pop_by_size (buffer_queue *buf, char *block, int size)
{
  char *pos = block;
  int popped = 0;
  buffer_packet *bp = fifo_peek(buf->queue);

  if (bp && bp->eof)
    return -1;

  while (popped < size)
  {
    if ((bp = fifo_peek(buf->queue)) == NULL)
      break;

    if (bp->eof)
      break;

    if (size-popped >= bp->length)
    {
      memcpy(pos, bp->content + bp->start, bp->length);
      popped += bp->length;
      pos += bp->length;

      fifo_pop(buf->queue);
      sfree(bp->content);
      sfree(bp);
    }
    else
    {
      memcpy(pos, bp->content + bp->start, size-popped);
      bp->length -= size-popped;
      bp->start += size-popped;
      popped = size;
    }
  }
  buf->queue_size -= popped;
  return popped;
}

/**
  * The actual work of this is done in buffer_pop_by_size, this function just determines the size.
  * @return Returns number of bytes writen to block.
  * -1 means eof flag encountered before delim
  * -2 means neither delim nor eof is present in the queue
  * -3 means your buffer wasn't big enough to hold all data up to requested delim.
  * Consider -1 fatal, -2 try again later, and -3 try again now with bigger buffer.
  */
int buffer_pop_to_delim (buffer_queue *buf, char *block, int size, char delim)
{
  fifo_link *fl = buf->queue->first;
  buffer_packet *bp;
  int scanned = 0;
  char *check;

  if (buf->queue_size == 0)
    return -1;

  do
  {
    bp = fl->data;

    if (bp->eof)
      return -1;

    check = memchr(bp->content + bp->start, delim, bp->length);
    if (check)
    {
      scanned += (check - (bp->content + bp->start)) + 1;
      break;
    }
    else
      scanned += bp->length;
  } while (fl = fl->next);

  if (fl == NULL)
    return -2;

  if (scanned > size)
    return -3;

  return buffer_pop_by_size(buf, block, scanned);
}

char buffer_peek (buffer_queue *buf)
{
  buffer_packet *bp = fifo_peek(buf->queue);
  if (bp->eof)
    return NULL;
  else
    return bp->content[bp->start];
}

int buffer_empty (buffer_queue *buf)
{
  buffer_packet *bp;
  int ret = 0;
  while (bp = fifo_pop(buf->queue))
  {
    if (bp->eof)
      ret = -1;
    else
      sfree(bp->content);
    sfree(bp);
  }
  buf->queue_size = 0;
  return ret;
}

void buffer_free (buffer_queue *buf)
{
  buffer_empty(buf);
  fifo_free(buf->queue);
  sfree(buf);
}
