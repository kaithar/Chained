/**
 * @file buffer.h
 * Chained in Sanity
 * Data types.
 * This file contains the buffer struct and it's typedef.
 */

#ifndef _INCLUDES_BUFFER_H_
#define _INCLUDES_BUFFER_H_

typedef struct buffer_queue buffer_queue;
typedef struct buffer_packet buffer_packet;

struct buffer_packet
{
  char *content;
  int start;
  int length;
  char eof;
};

/**
 * This struct abstracts a packet orientated buffer.
 */
struct buffer_queue
{
  /** This is a fifo of messages received from this connection but not processed yet. */
  fifo_root *queue;

  /** This is how much data we're storing on this recvq fifo */
  int queue_size;

  /** Block size, the amount of memory in each block */
  unsigned int block_size;
};

extern buffer_queue *new_buffer_queue (int block_size);
extern void buffer_store (buffer_queue *buf, char *block, int size);
extern void buffer_eof (buffer_queue *buf);
extern int buffer_pop_by_size (buffer_queue *buf, char *block, int size);
extern int buffer_pop_to_delim (buffer_queue *buf, char *block, int size, char delim);
extern char buffer_peek (buffer_queue *buf);
extern int buffer_empty (buffer_queue *buf);
extern void buffer_free (buffer_queue *buf);


#endif
