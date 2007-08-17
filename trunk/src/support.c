
#include <chained.h>

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
