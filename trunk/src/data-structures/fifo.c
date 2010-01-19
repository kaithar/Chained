/*
 * Chained
 *
 * FIFO management.
 */

#include "libchained/chained.h"

/*
 * First function is create, very simplistic really
 */
fifo_root *fifo_create()
{
	fifo_root *new = smalloc(sizeof(fifo_root));
	return new;
}

/*
 * Second we wish to be able to add ...
 * this function is a void return since either it suceeds or the program explodes
 * I like to call this "concusive feedback"
 *
 * Don't feed these functions NULL pointers or they will bite you.
 */
void fifo_add(fifo_root *parent, void *data)
{
	fifo_link *new = smalloc(sizeof(fifo_link));

	/* Oh dear, you seem to have pressed the self destruct button. */
	if ((parent == NULL) || (data == NULL))
		abort();

	/* To make things simple we're going to just bang this on the end.
	 * If you want something sorted you're probably after sortedlist_* */
	if (parent->last == NULL)
	{
		parent->first = new;
		parent->last = new;
	}
	else
	{
		parent->last->next = new;
		parent->last = new;
	}

	new->data = data;

	new->fifo = parent;
	parent->members++;
}

/*
 * Third, removal ...
 * We return an int ... how many of data we removed from the list ... though I should 2x4 you for adding multiples
 *
 * As before, NULL pointers will result in "concusive feedback"
 */
int fifo_del(fifo_root *fifo, void *data)
{
	fifo_link *curr = NULL;
	fifo_link *last = NULL;
	fifo_link *temp = NULL;
	int found = 0;

	/* 5 ... 4 ... 3 ... 2 ... 1 ... splat! */
	if ((fifo == NULL) || (data == NULL))
		abort();

	/* Alas, to loop we must. */
	for (curr = fifo->first; curr; )
	{
		if (curr->data == data)
		{
			/* bite! */
			if (curr->next == NULL)
				fifo->last = last;

			if (fifo->first == curr)
				fifo->first = curr->next;
			else
				last->next = curr->next;

			/* Finish the sucker off!
			 * Todo: Some sort of free'd list? Save a few malloc/free calls?
			 */
			temp = curr->next;
			free(curr);
			curr = temp;
			found++;
			fifo->members--;
			
			continue;
		}
		last = curr;
		curr = curr->next;
	}

	return found;
}

/*
 * Next it might be an idea to have something to free a fifo ... just in case?
 *
 * Feed this a null pointer and you deserve your feedback
 *
 * return is the number of links freed
 */
int fifo_free(fifo_root *fifo)
{
	fifo_link *curr = NULL;
	fifo_link *temp = NULL;
	int freed = 0;

	/* WuckFit! */
	if (fifo == NULL)
		abort();

	/* Here we go loop the loop... */
	if (fifo->first != NULL)
	{
		/*
		 * To save effort, work an object in hand ...
		 * Just so I can follow this when asleep:
		 */
		/* If there is only 1 we avoid the loop completely and just go to last free */
		if (fifo->first->next)
		{
			temp = fifo->first;
			curr = fifo->first->next;

			for (; curr != NULL; )
			{
				free(temp);
				freed++;

				temp = curr;
				curr = curr->next;
			}

			free(temp);
			freed++;
		}
		else	/* if (fifo->first->next) */
		{
			free(fifo->first);
			freed++;
		}
	}		/* if (fifo->first != NULL) */

	/* Lose the fifo itself */
	free(fifo);
	return freed;
}

/*
 * Should probably have a way to get data out again too...
 */

void *fifo_peek(fifo_root *fifo)
{
	if (fifo == NULL)
		abort();
	if (fifo->first == NULL)
		return NULL;
	return fifo->first->data;
}

void *fifo_pop(fifo_root *fifo)
{
	fifo_link *front = NULL;
	void *data = NULL;

	if (fifo == NULL)
		abort();

	front = fifo->first;

	if (front == NULL)
		return NULL;

	/* If the fifo actually has content, we pop the first one */
	if (fifo->first != NULL)
		fifo->first = fifo->first->next;

	/* If the fifo is now empty, remember to correct the tail pointer */
	if (fifo->first == NULL)
		fifo->last = NULL;

	/* Clean up after ourselves */
	data = front->data;
	free(front);
	fifo->members--;
	return data;
}

/*
 * Since it may also be of use, lets provide a push back function
 */

void fifo_push(fifo_root *parent, void *data)
{
	fifo_link *new = smalloc(sizeof(fifo_link));

	/* Oh dear, you seem to have pressed the self destruct button. */
	if ((parent == NULL) || (data == NULL))
		abort();

	if (parent->first == NULL)
	{
		parent->first = new;
		parent->last = new;
	}
	else
	{
		new->next = parent->first;
		parent->first = new;
	}

	new->data = data;

	new->fifo = parent;
	parent->members++;
}

/*
 * This function prints the memory addresses of the links and content of a fifo ...
 * It probably shouldn't be used unless you're debugging something.
 */
void fifo_print(fifo_root *p)
{
	fifo_link *temp = p->first;
  printf("L: ");
	for (;temp;temp = temp->next)
		printf("%d > ",temp);
	printf("\nD: ");

	temp = p->first;
	for (;temp;temp = temp->next)
		printf("%d > ",temp->data);
	printf("\n");
}

