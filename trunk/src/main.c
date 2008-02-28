/**
 * @file main.c
 * @brief Main setup stuff.
 * Chained in Sanity.
 * Core Library.
 * This is where the main core stuff happens
 */

#include "chained.h"
#include <sys/resource.h> /* For rlimit stuff */
#include <string.h>

/* Internal prototypes */
void cis_init_events();
void cis_init_modules();

/**
 * @brief I am set by config param "debug"
 * @internal
 * @todo This should probably be adjusted to a bitmask or something
 */

int DEBUG = 0; // Set by the config keyword "debug" ... no params

/**
 * @brief I am set by the socket engine of your choice
 * @internal
 */
socket_engine *socketengine = NULL;

/**
 * @brief Connection tracking...
 * @internal
 * @todo FIXME: Is this still needed or can it be removed?
 */
connection *connections[__MAXFDS__];

/**
 * @brief Would you like me to be fair?
 * @internal
 */
static int equal_fairness = 1;

/**
 * @brief Toggle Equal Fairnes on and off.
 * @return int: New queue mode: 0 = first come first served, 1 = equal fairness.
 */
int cis_toggle_fairness ()
{
	return (equal_fairness = (equal_fairness ? 0 : 1));
}

/**
 * @brief Reaper...
 * This is where we store sockets we're waiting to reap.
 * Using a fifo here would have been nice, but we really do need to be able to iterate.
 * @internal
 */

static linklist_root *reaper_list = NULL;

/**
 * @brief I add a connection to the kill list
 * If you force the code to reap a connection twice, it will most likely do a segfault.
 */

void cis_reap_connection(connection *cn)
{
	if (cn->state.reaping == 1)
		return;
	linklist_add(reaper_list,cn);
	cn->state.reaping = 1;
}

/**
 * @brief I try to make sure we're dropping cores...
 * This function attempts to make sure we drop cores.
 * Should it fail, it will generate an error to the stderr.
 * @todo
 * Ideally, we need some way of making error here fatal.
 * That way we can get the program to halt if it won't be able to drop cores.
 * @note
 * Thankie w00t for pointing this out :)
 */

void cis_drop_cores (void)
{
	struct rlimit rlim;
	memset(&rlim,0,sizeof(rlim));
	if (!getrlimit(RLIMIT_CORE,&rlim)) {
		rlim.rlim_cur = rlim.rlim_max;
		if (setrlimit(RLIMIT_CORE,&rlim)) {
			perror("rlimit");
			fprintf(stderr,"setting the resource limit failed with errno: %d\n",errno);
		}
	}
	return;
}

/**
 * @brief I initialize the various componants to make them ready for use.
 * This function should be called very early on, as it make the library ready for use.
 * Amongst other things it calls srand and mallocs some stuff.
 */

void cis_init (void)
{
	static initd = 0;
	
	if (initd == 1)
		fprintf(stderr,"You attempted to initalize the library twice!\n");
	
	srand(time(0));
	memset(connections,0,sizeof(connections));
	
	cis_init_events();
	cis_init_modules();
	
	reaper_list = linklist_create();
	
	initd = 1;
}

/**
 * @brief I make the magic happen... I am the main loop!
 * This function contains the main "reactor" loop...\n
 * The loop that makes it all happen basically.
 * The following things get called from this function:
 * - Socket engine
 * - Timer loop
 * - Connection Reaper
 * @todo
 * -# Although we can listen for write on a socket, the socket loop doesn't do anything with it.
 * -# Infact, the whole socket loop should probably be slimed down!  This function needs to be simple!
 */

void cis_run (void)
{
	int temp_int = 0;
	int i = 0, r, w;
	int eventcount = 0;
	connection *read_events[__MAXFDS__];
	connection *write_events[__MAXFDS__];
	
	fifo_root *global_recvq = NULL;
	linklist_iter *reaper_iter = NULL;
	
	connection *temp = NULL;
	char *line;
	
	int patience = 250;
	
	global_recvq = fifo_create();

	/* main loop */
	for(;;) {
		
		/** Grab some socket events to play with */
		eventcount = socketengine->wait(read_events, write_events, 250);
		r = w = 0;
		
		/** Run through the existing connections looking for data to read */
		for(i = 0; (r+w) < eventcount; i++) {
			if (read_events[i] != NULL)
			{
				r++;
				temp_int = read_events[i]->recvq_size;
				conn_read_to_recvq(read_events[i]);
				if ((read_events[i]->recvq_size > temp_int) && ((equal_fairness == 0) || (temp_int == 0)))
					fifo_add(global_recvq, read_events[i]);
				/* Don't wait for data! too much to do! */
				patience = 0;
			}
			if (write_events[i] != NULL)
			{
				w++;
				conn_send_from_sendq(write_events[i]);
			}
		} // foreach (event)
		
		/** Process some of the readq */
		
		if (global_recvq->members > 0)
		{
			for (i = 0; i <= 20; )
			{
				temp = fifo_pop(global_recvq);
				if (temp == NULL)
				{
					/* We seem to be out of connections to process... Have more patience waiting for new data ...*/
					patience = 250;
					break;
				}
				if ((temp->state.local_dead == 0)&&(temp->recvq->members > 0))
				{
					/* Local dead connections don't get processed ... remote dead ones -do- (since they died after sending this...) */
					line = fifo_pop( temp->recvq );
					
					if (temp->callback_read)
						temp->callback_read(temp,line);
					
					temp->recvq_size -= strlen(line);
					if (temp->recvq_size < 0)
						fprintf(stderr,"Um, negative recvq? Something is seriously wrong here.\n");
					
					/* If it doesn't have any more messages left, nuke it from the queue... */
					if (temp->recvq->members == 0)
					{
						fifo_del(global_recvq, temp);
						if (temp->state.remote_dead == 1)
							cis_reap_connection(temp);
					}
					else
					{
						if (equal_fairness == 1)
						{
							fifo_del(global_recvq, temp);
							fifo_add(global_recvq, temp);
						}
						else
						{
							/* We shove it back on the end of the queue just in case there are insufficent instances to cover the buffer content.
							 * While this behaviour won't break the code, it isn't true first come first served, and it will have a performance impact.
							 * TODO: It would be nice to have an alternative ... possibly making conn_read_to_recvq return a line count to allow multiple additions
							 */
							fifo_add(global_recvq, temp);
						}
					}
					
					free(line);
					
					if (global_recvq->members == 0)
					{
						patience = 250; /* Nothing to do, so I don't mind wait a while */
					}
					break;
				}
				else
				{
					/* So this is either locally dead, or has no recvq ... both could happen, but ignore it either way */
					if (temp->recvq->members > 0)
					{
						/* Must be locally dead but with a recvq still ... this shouldn't be possible? */
						discard_recvq(temp);
					}
					
					/* In any case, if we're ignoring it, might as well remove it completely ... */
					fifo_del(global_recvq, temp);
					
					/* Since we didn't do much with this one, we'll try another pass... */
					i--;
				}
			}
		}
		
		/** Attempt to reap connections... */
		reaper_iter = linklist_iter_create(reaper_list);
		while (temp = linklist_iter_next(reaper_iter))
		{
			/* Remove it from any relevant queues... */
			linklist_iter_del(reaper_iter);
			fifo_del(global_recvq,temp);
			/* Make sure it's all closed down... */
			temp->close(temp);
			/* Free the important stuff.... */
			if (temp->name)				free(temp->name);
			if (temp->recvq)			fifo_free(temp->recvq);
			if (temp->recvq_buf)	free(temp->recvq_buf);
			if (temp->sendq)			fifo_free(temp->sendq);
			if (temp->sendq_buf)	free(temp->sendq_buf);
			free(temp);
		}
		linklist_iter_free(reaper_iter);

		//processTimers();
	}
	return;
}


#if 0
// Special function, provides a wrapper for the `load`  config keyword
int config_keyword_core_c_debug(config_context *ctx, char *ignored) {
  // Expecting nothing, ignore anything;
	DEBUG = 1;
	printf("Debug mode now on!\n");
	return 0;
}
// Special function, for establishing config keywords from this file.
void config_keywords_core_c_export () {
	registerContextKeyword (NULL,"debug", &config_keyword_core_c_debug);
}
#endif
