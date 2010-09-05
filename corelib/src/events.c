/**
 * Chained
 * Event/Hook subsystem.
 * Fun for all the family ;)
 * @todo Comment this damned file!
 */

#include <stdarg.h> /* va_*() */

#include "libchained/chained.h"

map_root *hooks_map = NULL;

void cis_init_events()
{
	if (hooks_map != NULL)
		abort();

	hooks_map = map_create();
	map_allow_preset(hooks_map, ALLOWED_NICK_CHARS);
	/* If you want more, add them here... */
	map_allow_char(hooks_map, '+');
	map_allow_char(hooks_map, ' ');
	map_allow_char(hooks_map, '=');
	map_allow_char(hooks_map, '*');
	map_allow_char(hooks_map, '/');
	map_allow_char(hooks_map, '(');
	map_allow_char(hooks_map, ')');
	map_init(hooks_map);
}

static event *cis_event_create(unsigned char *event_name)
{
	event *temp = NULL;
	linklist_root *hooks = NULL;

	if ((map_validate_key(hooks_map, event_name) == false) || (strlen((char *)event_name) > 100))
		abort();
	
	if ((temp = map_find(hooks_map,event_name)) == NULL)
	{
		temp = smalloc(sizeof(event));
		snprintf((char *)temp->event, 100, "%s", event_name);
		
		hooks = linklist_create();
		
		temp->hooks = hooks;
		
		map_add(hooks_map, event_name, temp);
	}
	
	return temp;
}

hook *cis_event_hook(unsigned char *event_name, int (*callback)(int version, int parc, void **parv))
{
	hook *temp = NULL;
	event *temp_event = NULL;
	
	temp_event = cis_event_create(event_name);

	temp = smalloc(sizeof(hook));
	snprintf((char *)temp->event, 100, "%s", event_name);
	temp->callback = callback;
	
	linklist_add(temp_event->hooks, temp);
	
	return temp;
}

event *cis_event_ref(unsigned char *event_name)
{
	event *temp = NULL;
	
	temp = cis_event_create(event_name);
	temp->references++;
	
	return temp;
}

/* The version param is intended to allow event hooks to fail gracefully on param changes
 * The theory is that a module can log a warning and return continue if version is unexpected. 
 */
int cis_event_do(event *event_obj, int version, int parc, ...)
{
	int status = 0;
	hook *hook_obj;
	linklist_iter *iter = NULL;
	
	va_list argv;
	void *parv[16] = { NULL };
	int i = 0;

	if (event_obj == NULL)
		abort();

	/* We only accept 15 parameters through this ...
	 * If you feel the need, increase this check and the parv array to raise that limit
	 * parv's size should be one more than this one to allow for a null pointer
	 */
	if (parc > 15)
		abort();
		
	va_start(argv, parc);
	
	for (i = 0;i < parc;i++)
	{
		parv[i] = (void *)va_arg(argv,void *);
	}
	parv[parc] = NULL;
	va_end(argv);

	iter = linklist_iter_create(event_obj->hooks);
	while ((hook_obj = linklist_iter_next(iter)) != NULL)
	{
		status = hook_obj->callback(version, parc, parv);

		if (status == EVENT_DENY)
		{
			linklist_iter_free(iter);
			return EVENT_DENY;
		}
		else if (status == EVENT_OVERRIDE)
			break;
	}

	linklist_iter_free(iter);

	return EVENT_CONTINUE;
}

int cis_event_remove_hook(hook *hook)
{
	event *event_obj;

	event_obj = cis_event_create(hook->event);
	
	linklist_del(event_obj->hooks,hook);
	
	free(hook);
	
	return true;
}

int cis_event_release(event *event_obj)
{
	event_obj->references--;
	if ((event_obj->references == 0)&&(event_obj->hooks->members == 0))
	{
		/* Event no longer used... */
		map_del(hooks_map, event_obj->event);
		linklist_free(event_obj->hooks);
		free(event_obj);
	}
	return true;
}
