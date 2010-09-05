/**
 * @file timers.h
 * @brief This file contains the timer_event definition.
 */

#ifndef _INCLUDES_TYPES_TIMERS_H
#define _INCLUDES_TYPES_TIMERS_H

typedef struct timer_event timer_event;

/**
 * Timer event structure.
 */
struct timer_event
{
	timer_event *prev, *next;                /**< Prev/Next event */
	unsigned int when;                       /**< When does this timer trigger? */
	int delay;                               /**< Delay before retriggering */
	int repetitions;                         /**< How many times to do this event */
	void *callback_param;                    /**< param for callback */
	int (*callback)(timer_event *, void*);   /**< the callback for this event, void* is some sort of value meaningful to the callback */
};

#endif
