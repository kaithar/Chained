/* 
 * Mythica
 * Timers header file
 */

typedef struct struct_timer_event timer_event;

struct struct_timer_event
{
	timer_event *prev, *next;		/* Prev/Next event */
	unsigned int when;			/* When does this timer trigger? */
	int delay;				/* Delay before retriggering */
	int repetitions;			/* How many times to do this event */
	int callback_param;			/* param for callback */
	int (*callback)(timer_event *, int);	/* the callback for this event, int is some sort of value meaningful to the callback */
};

int timers_add(timer_event *);
int timers_del(timer_event *);
int timers_process();
