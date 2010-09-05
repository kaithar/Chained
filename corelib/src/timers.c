/**
 * @file timers.c
 * @brief Timed events, rather important.
 * Chained in Sanity.
 * Core Library.
 * Code for handling timed events
 * We don't use linklist_*() here because we require an int sorted list...
 */

#include "libchained/chained.h"

// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <math.h>
// #include <sys/types.h>
// #include <time.h>

static timer_event *firstevent = 0;
static timer_event *lastevent = 0;
static int eventcount = 0;

int cis_timers_add(timer_event *ev)
{
	timer_event *curr;
	int inserted = 0;

	if (lastevent == 0)
	{
		lastevent = ev;
		firstevent = ev;
	}
	else
	{
		for (curr = lastevent; curr; curr = curr->prev)
		{
			if (ev->when > curr->when)
			{	/* Insert ev after curr */
				if (curr->next == 0)
				{	/* last one */
					ev->next = 0;
					lastevent = ev;
				}
				else
				{
					/*       .--. */
					/* C   EV   O */
					curr->next->prev = ev;
					ev->next = curr->next;
				}
	
				/*  .--. */
				/* C   EV   O */
				curr->next = ev;
				ev->prev = curr;
				inserted = 1;
				
				break;
			}	/* if (insert here) */
		}

		if (!inserted)
		{
			/* Seems we didn't manage to insert it, so we should probably pop it in the begining */
			ev->prev = 0;
			ev->next = firstevent;
			firstevent->prev = ev;
			firstevent = ev;
			inserted = 1;
		}
	}			/* if (lastevent != 0) */

	eventcount++;
	return 1;
}

int cis_timers_del(timer_event *ev)
{
	if (ev->next)
	{
		ev->next->prev = ev->prev;
	}
	else
	{
		lastevent = ev->prev;
	}
	
	if (ev->prev)
	{
		ev->prev->next = ev->next;
	}
	else
	{
		firstevent = ev->next;
	}
	
	eventcount--;
	
	return 1;
}

int cis_timers_process()
{
	timer_event *curr;
	timer_event *temp;
	unsigned int nowish = 0;
	
	if (firstevent == 0)
	{
		/* Assumably there are no events, return a 0; */
		return 0;
	}
	else
	{
		nowish = time(0);
	
		for (curr = firstevent; curr;)
		{
			/*
			 * Loop from the begining ...
			 * this won't loop for long, we just wanna check events that have past, and they are in cronological order :)
			 */
			if (nowish >= curr->when)
			{	/* Time to do this (gotta do timers that have already passed too) */
				curr->callback(curr, curr->callback_param);
		
				if (curr->repetitions == 1)
				{
					/* last rep, remove it please */
					temp = curr;
					curr = curr->next;
					cis_timers_del(temp);
					free(temp);
				}
				else
				{
					/* Remove and ReAdd to get it in the right place please :) */
					temp = curr;
					curr = curr->next;

					if (temp->repetitions > 0)
					{	/* Don't repeat adnausiem */
						temp->repetitions--;
					}
					
					temp->when += temp->delay;
					cis_timers_del(temp);
					cis_timers_add(temp);
				}	/* else for if (reps == 1) */
				/* Now at this point in time, curr should have moved on to the next one */
			}
			else
			{	/*
				 * if (do timers in the past)
				 * current has yet to reach now...
				 */
				break;
			}
		}
	}			/* if (firstevent != 0) */

	return 0; /* XXX: make this void? */
}
