
#include "libchained/chained.h"

void  *smalloc(int size)
{
	void *p;

	/* oh to have beat_coder_over_head_with_2_by_4() ... guess this will have to do ...*/
	if (size <= 0)
		abort();
  
	p = malloc(size);

	/* die_screaming_in_much_agony_while_being_eaten_by_small_dogs() would be nice... */
	if (p == NULL)
		abort();

	memset(p,0,size);
	return (p);
}

void sfree(void *mem)
{
  free(mem);
}

/******************************/

/** Do blocking exec ...*/

int cis_do_blocking_exec (unsigned char *file, unsigned char *buffer, unsigned int n) 
{
  unsigned int pnbytes = 0;
  FILE *f;
  if ((f = popen(file, "r")) == NULL)
    return -1;
  if ((pnbytes = fread(buffer, sizeof(char), n-1, f)) == 0)
  {
    pclose(f);
    return -2;
  }
  buffer[pnbytes] = '\0';
  pclose(f);
  return pnbytes;
}

/******************************/

void discard_sendq (connection *conn)
{
	char *temp = NULL;
	while (temp = fifo_pop(conn->sendq))
		free(temp);
	conn->sendq_size = 0;
}

void discard_recvq (connection *conn)
{
	char *temp = NULL;
	while (fifo_pop(conn->recvq))
		free(temp);
	conn->recvq_size = 0;
}
