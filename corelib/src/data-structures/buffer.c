/**
 * @file buffer.c
 * @brief Represents packets of data.
 * Chained in Sanity.
 * Data structures.
 * Controls a buffer object.
 * Abstractions to queue to present as a continue data stream.
 */

#include "includes/chained.h"
#include <string.h>

buffer_queue *new_buffer_queue (unsigned int block_size)
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
void buffer_store (buffer_queue *buf, char *block, unsigned int size)
{
  buffer_packet *bp;
  char *pos = block;
  unsigned int copy_chunk;
  buf->queue_size += size;
  while (size > 0)
  {
    bp = smalloc(sizeof(buffer_packet));
    bp->content = smalloc((size_t) buf->block_size);
    if (size > buf->block_size)
      copy_chunk = buf->block_size;
    else
      copy_chunk = size;
    memcpy(bp->content, pos, (size_t)copy_chunk);
    bp->length = copy_chunk;
    bp->start = 0;
    bp->eof = false;
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
  bp->eof = true;
  fifo_add(buf->queue, bp);
}

/** buffer_pop_chunk - Get next chunk.
 * Useful mainly for UDP and ICMP connections, which have a chunk to record equivilence.
 * @return Returns number of bytes writen to block.
 *  0 means block of 0 bytes encountered and written to block.
 * -1 means eof encountered.
 * -2 means queue is empty
 * -3 means your buffer wasn't big enough to hold all data up to requested delim.
 * Consider -1 fatal, -2 try again later, and -3 try again now with bigger buffer.
 */

int buffer_pop_chunk(buffer_queue *buf, char *block, unsigned int size)
{
	unsigned int ret = 0;
	buffer_packet *bp = fifo_peek(buf->queue);
	if ((bp != NULL) && bp->eof)
    return -1;
	if (bp == NULL)
		return -2;
	if (bp->length > size)
		return -3;
	memcpy(block, bp->content + bp->start, (size_t)bp->length);
	ret = bp->length;
	(void) fifo_pop(buf->queue);
	sfree(bp->content);
	sfree(bp);
	return ret;
}

int buffer_pop_by_size (buffer_queue *buf, char *block, unsigned int size)
{
  char *pos = block;
  unsigned int popped = 0;
  buffer_packet *bp = fifo_peek(buf->queue);

  if ((bp != NULL) && bp->eof)
    return -1;

  while (popped < size)
  {
    if ((bp = fifo_peek(buf->queue)) == NULL)
      break;

    if (bp->eof)
      break;

    if (size-popped >= bp->length)
    {
      memcpy(pos, bp->content + bp->start, (size_t)bp->length);
      popped += bp->length;
      pos += bp->length;

      (void) fifo_pop(buf->queue);
      sfree(bp->content);
      sfree(bp);
    }
    else
    {
      memcpy(pos, bp->content + bp->start, (size_t)size-popped);
      bp->length -= size-popped;
      bp->start += size-popped;
      popped = size;
    }
  }
  buf->queue_size -= popped;
  return (int) popped;
}

/**
  * The actual work of this is done in buffer_pop_by_size, this function just determines the size.
  * @return Returns number of bytes writen to block.
  * -1 means eof flag encountered before delim
  * -2 means neither delim nor eof is present in the queue
  * -3 means your buffer wasn't big enough to hold all data up to requested delim.
  * Consider -1 fatal, -2 try again later, and -3 try again now with bigger buffer.
  */
int buffer_pop_to_delim (buffer_queue *buf, char *block, unsigned int size, char delim)
{
  fifo_link *fl = buf->queue->first;
  buffer_packet *bp;
  unsigned int scanned = 0;
  char *check;

  if (buf->queue_size == 0)
    return -1;

  do
  {
    bp = fl->data;

    if (bp->eof)
      return -1;

    check = memchr(bp->content + bp->start, (int)delim, (size_t)bp->length);
    if (check)
    {
      scanned += (check - (bp->content + bp->start)) + 1;
      break;
    }
    else
      scanned += bp->length;
  } while ((fl = fl->next));

  if (fl == NULL)
    return -2;

  if (scanned > size)
    return -3;

  return (int) buffer_pop_by_size(buf, block, scanned);
}

char buffer_peek (buffer_queue *buf)
{
  buffer_packet *bp = fifo_peek(buf->queue);
  if (bp->eof)
    return '\0';
  else
    return bp->content[bp->start];
}

int buffer_empty (buffer_queue *buf)
{
  buffer_packet *bp;
  int ret = 0;
  while ((bp = fifo_pop(buf->queue)))
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
  (void) buffer_empty(buf);
  (void) fifo_free(buf->queue);
  sfree(buf);
}
