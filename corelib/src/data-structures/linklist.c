/*
 * Chained
 *
 * Link list management.
 */

#include "includes/chained.h"

/*
 * Link lists ...
 * first function is create, very simplistic really
 */
linklist_root *linklist_create()
{
	linklist_root *new = smalloc(sizeof(linklist_root));
	return new;
}

/*
 * Second we wish to be able to add ...
 * this function is a void return since either it suceeds or the program explodes
 * I like to call this "concusive feedback"
 *
 * Don't feed these functions NULL pointers or they will bite you.
 */
void linklist_add(linklist_root *parent, void *data)
{
	linklist_link *new = smalloc(sizeof(linklist_link));

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
		new->prev = parent->last;
		parent->last = new;
	}

	new->data = data;

	new->list = parent;
	parent->members++;
}

/*
 * Third, removal ...
 * We return an int ... how many of data we removed from the list ... though I should 2x4 you for adding multiples
 *
 * As before, NULL pointers will result in "concusive feedback"
 */
int linklist_del(linklist_root *list, void *data)
{
	linklist_link *curr = NULL;
	linklist_link *temp = NULL;
	int found = 0;

	/* 5 ... 4 ... 3 ... 2 ... 1 ... splat! */
	if ((list == NULL) || (data == NULL))
		abort();

	/* Alas, to loop we must. */
	for (curr = list->first; curr;)
	{
		if (curr->data == data)
		{
			/* bite! */
			if (curr->next != NULL)
				curr->next->prev = curr->prev;
			else
				list->last = curr->prev;

			if (curr->prev != NULL)
				curr->prev->next = curr->next;
			else
				list->first = curr->next;
			
			/* Finish the sucker off!
			 * Todo: Some sort of free'd list? Save a few malloc/free calls?
			 */
			temp = curr->next;
			free(curr);
			curr = temp;
			found++;
			list->members--;
			
			continue;
		}

		curr = curr->next;
	}

	return found;
}

/*
 * Next it might be an idea to have something to free a linked list ... just in case?
 *
 * Feed this a null pointer and you deserve your feedback
 *
 * return is the number of links freed
 */
int linklist_free(linklist_root *list)
{
	linklist_link *curr = NULL;
	int freed = 0;

	/* WuckFit! */
	if (list == NULL)
		abort();

	/* Here we go loop the loop... */
	if (list->first != NULL)
	{
		/*
		 * To save effort, work an object in hand ...
		 * Just so I can follow this when asleep:
		 */
		/* If there is only 1 we avoid the loop completely and just go to last free */
		if (list->first->next)
		{
			curr = list->first->next;

			for (;;)
			{
				free(curr->prev);
				freed++;
			
				if (curr->next)
					curr = curr->next;
				else
					break;

			}

			free(curr);
			freed++;
		}
		else	/* if (list->first->next) */
		{
			free(list->first);
			freed++;
		}
	}		/* if (list->first != NULL) */

	/* Loose the list itself */
	free(list);
	return freed;
}

/*
 * Now that we have a linked list to work with, we should probably have a way to iterate through it ... that's what this is for...
 *
 * Do I need to repeat myself? NULL == boom
 *
 * Note an iterator starts off point at NULL link ... next will put it on first, prev will put it on last.
 */
linklist_iter *linklist_iter_create(linklist_root *list)
{
	linklist_iter *new = NULL;

	/* Subtle? I can do subtle... */
	if (list == NULL)
		abort();

	new = smalloc(sizeof(linklist_iter));
	new->list = list;

	return new;
}

/*
 * So what do we do with this iterator? Well a next would be good
 *
 * Blah blah NULL blah bad blah
 */
void *linklist_iter_next(linklist_iter *iter)
{
	if (iter == NULL)
		abort();

	if (iter->current == NULL)
		iter->current = iter->list->first;
	else
		iter->current = iter->current->next;

	if (iter->current == NULL)
		return NULL;
	else
		return iter->current->data;
}

/*
 * Perhaps you wanna move backward?
 */
void *linklist_iter_prev(linklist_iter *iter)
{
	if (iter == NULL)
		abort();

	if (iter->current == NULL)
		iter->current = iter->list->last;
	else
		iter->current = iter->current->prev;

	if (iter->current == NULL)
		return NULL;
	else
		return iter->current->data;
}

/*
 * Or maybe just jump back to the start again?
 */
void linklist_iter_seek_start(linklist_iter *iter)
{
	if (iter == NULL)
		abort();

	iter->current = NULL;

}

/*
 * So you found the data you want ... now what?
 * Adiga ... adiga ... adiga ..... DEEEEELETE!
 *
 * For the terminally stupid, NULL still causes the baby jesus to implode.
 */
void linklist_iter_del(linklist_iter *iter)
{
	linklist_link *temp;

	if (iter == NULL)
		abort();

	/* You're having a laugh, right? */
	if (iter->current == NULL)
		return;

	if (iter->current->next != NULL)
		iter->current->next->prev = iter->current->prev;
	else
		iter->list->last = iter->current->prev;
	if (iter->current->prev != NULL)
		iter->current->prev->next = iter->current->next;
	else
		iter->list->first = iter->current->next;

	temp = iter->current->prev;
	free(iter->current);
	iter->current = temp;
	iter->list->members--;
}



/************************************************
 * Sorted Lists                                 *
 * Just like the above functions,               *
 * but with a little extra!                     *
 ************************************************/

/*
 * first function is create, no changes really
 */

slinklist_root *slinklist_create()
{
	slinklist_root *new = smalloc(sizeof(slinklist_root));
	return new;
}

/*
 * Adding ....
 * this function is a void return since either it suceeds or the program explodes
 * Yup, still got the "concusive feedback"
 *
 * Don't feed these functions NULL pointers or they will bite you.
 *
 * The logic here is slightly more complex than linklist_add since we're dealing with a sorted list
 */
bool slinklist_add(slinklist_root *parent, unsigned char *key, void *data)
{
	slinklist_link *new = smalloc(sizeof(slinklist_link));
	slinklist_link *ins;
	int foo = 0;

	/* Oh dear, you seem to have pressed the self destruct button. */
	if ((parent == NULL) || (key == NULL) || (data == NULL))
		abort();

	/* So ... use the index functions, then iterate til we have a match.  NULL index means insert at begining */
	if (parent->index[key[0]] == NULL)
	{
		if (parent->first == NULL)
		{
			parent->first = new;
			parent->last = new;
		}
		else
		{
			new->next = parent->first;
			parent->first->prev = new;
			parent->first = new;
		}
	}
	else			/* end if (parent->index[key[0]] == NULL) */
	{
		ins = parent->index[key[0]];

		for (;;)
		{
			foo = strcmp((char *)key, (char *)ins->key);
		
			if (foo < 0)
			{
				/* key is less than position, do insertion! */
				if (ins->prev == NULL)
				{	/* Annoy, but valid .. insert as first object */
					new->next = parent->first;
					parent->first->prev = new;
					parent->first = new;
				}
				else
				{
					/* Insert before ins */
					new->prev = ins->prev;
					new->next = ins;
					new->prev->next = new;
					new->next->prev = new;
				}
			
				break;
			}
			else if (foo == 0)
			{
				/* Duplicate key! */
				return false;
			}
			else
			{
				/* foo > 0 */
				if (ins->next)
				{
					ins = ins->next;
					continue;
				}
				
				/* We've reached the end of the line, insert please! */
				new->prev = parent->last;
				parent->last->next = new;
				parent->last = new;
				
				break;
			}
		}
	}

	/* Update index */

	/* Update current letter index */
	if (
			/* This is true when we insert the first entry: */
			(parent->index[*key] == NULL) ||
			/*
			 * When new is next after index,
			 * but doesn't start with the same letter
			 */
			((parent->index[*key] == new->prev) && (*new->prev->key != *key)) ||
			/*
			 * When new is just before index,
			 * and starts with the same letter
			 */
			((parent->index[*key] == new->next) && (*new->next->key == *key)) ||
			/* This is a fall through string compare */
			(strcmp((char *)key, (char *)parent->index[*key]->key) < 0))
	{
		/* Our new one got inserted before our current indexed "start point", or said index is for the previous letter */
		parent->index[*key] = new;
	}

	/* Update following letters */
	for (foo = ((int)*key) + 1; foo <= 255; foo++)
	{
		/*
		 * If we're NULL we update.
		 * If the index corrispondes to new->prev, we update (this is to account for adding to the end)

		 * These stated 2 rules will work for all cases. Any other rule will prove false and/or duplicate above.
		 * The first index not matching these 2 rules marks the point beyond which there will be 0 matches.
		 */
		if ((parent->index[foo] == NULL) || (parent->index[foo] == new->prev))
			parent->index[foo] = new;
		else
			break;
	}

	new->key = (unsigned char *)strdup((char *)key);
	new->data = data;

	new->list = parent;
	parent->members++;

	return true;
}

/*
 * Third, removal ...
 * We return an int ... how many of data we removed from the list ... note that we will not allow duplicate keys
 *
 * As before, NULL pointers will result in "concusive feedback"
 */
int slinklist_del_key(slinklist_root *list, unsigned char *key)
{
	slinklist_link *curr = NULL;
	int foo = 0;

	/* 5 ... 4 ... 3 ... 2 ... 1 ... splat! */
	if ((list == NULL) || (key == NULL))
		abort();

	/* Alas, to loop we must.
	 * Don't forget, if index[*key] is null, this loop is bypassed and we return 0
	 */
	for (curr = list->index[*key]; curr;)
	{
		foo = strcmp((char *)key, (char *)curr->key);

		if (foo == 0)
		{
			/* bite! */
			if (curr->next != NULL)
				curr->next->prev = curr->prev;
			else
				list->last = curr->prev;
		
			if (curr->prev != NULL)
				curr->prev->next = curr->next;
			else
				list->first = curr->next;
			
			/* How about attacking the index? */
			if (list->index[*key] == curr)
			{
				if ((curr->next != NULL) && (curr->next->key[0] == *key))
					list->index[*key] = curr->next;	/* We have a next and it's the same first letter, so set... */
				else
					list->index[*key] = curr->prev;	/* Otherwise, matter what curr->prev is, it is right */
			}
			
			for (foo = ((int)*key) + 1; foo <= 255; foo++)
			{
				/*
				 * If this index is curr, set it to curr->prev
				 * Otherwise break because we're not going to find any more matches
				 */
				if (list->index[foo] == curr)
					list->index[foo] = curr->prev;
				else
					break;
			}
			
			/*
			 * Finish the sucker off!
			 * Todo: Some sort of free'd list? Save a few malloc/free calls?
			 */
			free(curr->key);
			free(curr);
			list->members--;
			
			return 1;
		}
		else if (foo > 1)
		{
			/* If we've reached this point we've passed the point where we are likely to find a match. */
			return 0;
		}

		curr = curr->next;
	}

	return 0;
}

/*
 * Alternatively, since someone things it would be nice, delete by value.
 * Use delete by key if you can, it's more efficent by a mile.
 */
int slinklist_del_value(slinklist_root *list, void *data)
{
	slinklist_link *curr = NULL;
	slinklist_link *temp = NULL;
	int found = 0;
	int foo = 0;

	/* 5 ... 4 ... 3 ... 2 ... 1 ... splat! */
	if ((list == NULL) || (data == NULL))
		abort();

	/* Alas, to loop we must. No shortcuts here */
	for (curr = list->first; curr;)
	{
		if (curr->data == data)
		{
			/* bite! */
			if (curr->next != NULL)
				curr->next->prev = curr->prev;
			else
				list->last = curr->prev;

			if (curr->prev != NULL)
				curr->prev->next = curr->next;
			else
				list->first = curr->next;
			
			/* Our way of dealing with the index is same as by key */
			if (list->index[curr->key[0]] == curr)
			{
				if ((curr->next != NULL) && (curr->next->key[0] == curr->key[0]))
					list->index[curr->key[0]] = curr->next;	/* We have a next and it's the same first letter, so set... */
				else
					list->index[curr->key[0]] = curr->prev;	/* Otherwise, matter what curr->prev is, it is right */
			}
			
			for (foo = ((int)curr->key[0]) + 1; foo <= 255; foo++)
			{
				/*
				 * If this index is curr, set it to curr->prev
				 * Otherwise break because we're not going to find any more matches
				 */
				if (list->index[foo] == curr)
					list->index[foo] = curr->prev;
				else
					break;
			}
			
			/*
			 * Finish the sucker off!
			 * Todo: Some sort of free'd list? Save a few malloc/free calls?
			 */
			temp = curr->next;
			free(curr->key);
			free(curr);
			curr = temp;
			found++;
			list->members--;
	
			continue;
		}

		curr = curr->next;
	}

	return found;
}


/*
 * Next it might be an idea to have something to free a linked list ... just in case?
 *
 * Feed this a null pointer and you deserve your feedback
 *
 * return is the number of links freed
 */
int slinklist_free(slinklist_root *list)
{
	slinklist_link *curr = NULL;
	int freed = 0;

	/* WuckFit! */
	if (list == NULL)
		abort();

	/* Here we go loop the loop... */
	if (list->first != NULL)
	{
		/*
		 * To save effort, work an object in hand ...
		 * Just so I can follow this when asleep:
		 */
		/* If there is only 1 we avoid the loop completely and just go to last free */
		if (list->first->next)
		{
			curr = list->first->next;

			for (;;)
			{
				free(curr->prev->key);
				free(curr->prev);
				freed++;
				if (curr->next)
					curr = curr->next;
				else
					break;

			}
			
			free(curr->key);
			free(curr);
			freed++;
		}
		else	/* if (list->first->next) */
		{
			free(list->first->key);
			free(list->first);
			freed++;
		}
	}		/* if (list->first != NULL) */

	/* Loose the list itself */
	free(list);

	return freed;
}

/*
 * Now that we have a linked list to work with, we should probably have a way to iterate through it ... that's what this is for...
 *
 * Do I need to repeat myself? NULL == boom
 *
 * Note an iterator starts off point at NULL link ... next will put it on first, prev will put it on last.
 */
slinklist_iter *slinklist_iter_create(slinklist_root *list)
{
	slinklist_iter *new = NULL;

	/* Subtle? I can do subtle... */
	if (list == NULL)
		abort();

	new = smalloc(sizeof(slinklist_iter));
	new->list = list;

	return new;
}

/*
 * So what do we do with this iterator? Well a next would be good
 *
 * Blah blah NULL blah bad blah
 */
void *slinklist_iter_next(slinklist_iter *iter)
{
	if (iter == NULL)
		abort();

	if (iter->current == NULL)
		iter->current = iter->list->first;
	else
		iter->current = iter->current->next;

	if (iter->current == NULL)
		return NULL;
	else
		return iter->current->data;
}

/*
 * Perhaps you wanna move backward?
 */
void *slinklist_iter_prev(slinklist_iter *iter)
{
	if (iter == NULL)
		abort();

	if (iter->current == NULL)
		iter->current = iter->list->last;
	else
		iter->current = iter->current->prev;

	if (iter->current == NULL)
		return NULL;
	else
		return iter->current->data;
}

/*
 * Or maybe just jump back to the start again?
 */
void slinklist_iter_seek_start(slinklist_iter *iter)
{
	if (iter == NULL)
		abort();

	iter->current = NULL;

}

/*
 * So you found the data you want ... now what?
 * Adiga ... adiga ... adiga ..... DEEEEELETE!
 *
 * For the terminally stupid, NULL still causes the baby jesus to implode.
 */
void slinklist_iter_del(slinklist_iter *iter)
{
	slinklist_link *temp;
	int foo = 0;

	if (iter == NULL)
		abort();

	/* You're having a laugh, right? */
	if (iter->current == NULL)
		return;

	if (iter->current->next != NULL)
		iter->current->next->prev = iter->current->prev;
	else
		iter->list->last = iter->current->prev;

	if (iter->current->prev != NULL)
		iter->current->prev->next = iter->current->next;
	else
		iter->list->first = iter->current->next;

	/* Update index */
	if (iter->list->index[iter->current->key[0]] == iter->current)
	{
		if ((iter->current->next != NULL) && (iter->current->next->key[0] == iter->current->key[0]))
			iter->list->index[iter->current->key[0]] = iter->current->next;	/* We have a next and it's the same first letter, so set... */
		else
			iter->list->index[iter->current->key[0]] = iter->current->prev;	/* Otherwise, matter what curr->prev is, it is right */
	}
	
	for (foo = ((int)iter->current->key[0]) + 1; foo <= 255; foo++)
	{
		/*
		 * If this index is curr, set it to curr->prev
		 * Otherwise break because we're not going to find any more matches
		 */
		if (iter->list->index[foo] == iter->current)
			iter->list->index[foo] = iter->current->prev;
		else
			break;
	}

	temp = iter->current->prev;
	free(iter->current->key);
	free(iter->current);
	iter->current = temp;
	iter->list->members--;
}
