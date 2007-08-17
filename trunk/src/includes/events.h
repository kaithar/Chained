/*
 * Chained
 * Header for the events/hooks system
 */
 
#ifndef __events_include__
#define __events_include__

enum {
	/* Deny will generally cause the event to stop happening. */
	EVENT_DENY,
	/* Continue will make the event continue on it's merry way */
	EVENT_CONTINUE,
	/* Override will halt the processing of any more hooks and return a continue. */
	/* It's named Override to prevent any possible confusion.  It overrides the remaining hooks. */
	EVENT_OVERRIDE
};

typedef struct hook hook;
typedef struct event event;

struct hook
{
	unsigned char event[101];
	int (*callback)(int version, int parc, void **parv);
};

struct event
{
	unsigned char event[101];
	unsigned int references;
	linklist_root *hooks;
};

extern hook *cis_event_hook(unsigned char *event_name, int (*callback)(int version, int parc, void **parv));
extern int cis_event_do(event *event_obj, int version, int parc, ...);
extern int cis_event_remove_hook(hook *hook);

extern event *cis_event_ref(unsigned char *event_name);
extern int cis_event_release(event *event_obj);

#endif
