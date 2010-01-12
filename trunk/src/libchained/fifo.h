/*
 * Stuff for fifo.c
 */

#ifndef __fifo_include__
#define __fifo_include__

/*
 * Typedef section
 */
typedef struct fifo_root fifo_root;
typedef struct fifo_link fifo_link;

/*
 * Now for some structs
 */
struct fifo_root
{
	fifo_link *first, *last;
	int members;
};

struct fifo_link
{
	fifo_link *next;
	fifo_root *fifo;
	void *data;
};

/*
 * Prototype!
 */
extern fifo_root *fifo_create();
extern void fifo_add(fifo_root *parent, void *data);
extern int fifo_del(fifo_root *fifo, void *data);
extern int fifo_free(fifo_root *fifo);
extern void *fifo_peek(fifo_root *fifo);
extern void *fifo_pop(fifo_root *fifo);
extern void fifo_push(fifo_root *parent, void *data);

#endif
