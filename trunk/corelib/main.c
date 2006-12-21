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
 */
connection *connections[__MAXFDS__];

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
 * -# Write a fifo list so the hack used for recvq/sendq can be nuked!
 */

void cis_run (void)
{
	int i = 0, r, w;
	int eventcount = 0;
	connection *read_events[__MAXFDS__];
	connection *write_events[__MAXFDS__];
	
	linklist_root *global_recvq;
	linklist_iter *global_recvq_start;
	
	linklist_iter *hack = NULL;
	
	connection *temp = NULL;
	char *line;
	
	int patience = 250;
	
	global_recvq = linklist_create();
	global_recvq_start = linklist_iter_create( global_recvq );
	hack = linklist_iter_create( global_recvq );
	
	/* main loop */
	for(;;) {
		
		eventcount = socketengine->wait(read_events, write_events, 250);
		r = w = 0;
		
		/* Run through the existing connections looking for data to read */
		for(i = 0; (r+w) < eventcount; i++) {
			if (read_events[i] != NULL)
			{
				r++;
				conn_read_to_recvq(read_events[i]);
				linklist_add(global_recvq, read_events[i]);
				/* Don't wait for data! too much to do! */
				patience = 0;
			}
			if (write_events[i] != NULL)
			{
				w++;
				conn_send_from_sendq(write_events[i]);
			}
		} // foreach (event)
		
		if (global_recvq->members > 0)
		{
			for (;;)
			{
				temp = linklist_iter_next(global_recvq_start);
				if (temp == NULL)
				{
					/* We seem to be out of connections to process... Have more patience waiting for new data ...*/
					patience = 250;
					break;
				}
				if ((temp->state.dead == 0)&&(temp->recvq->members > 0))
				{
					/* Dead connections don't get processed ... closed ones -do- (since they may have closed after sending this...) */
					/* HACK! NEED A FIFO LIST! */
					hack->list = temp->recvq;
					hack->current = NULL;
					line = linklist_iter_next( hack );
					
					if (temp->callback_read)
						temp->callback_read(temp,line);
					
					temp->recvq_size -= strlen(line);
					if (temp->recvq_size < 0)
						fprintf(stderr,"Um, negative recvq? Something is seriously wrong here.\n");
					
					linklist_iter_del( hack );
					
					/* Only remove from the global recvq if we've finished processing it's messages */
					if (temp->recvq->members == 0)
						linklist_iter_del( global_recvq_start );
					
					free(line);
					
					if (global_recvq->members == 0)
					{
						patience = 250; /* Nothing to do, so I don't mind wait a while */
					}
					break;
				}
				else
				{
					/* So this is either dead, or has no recvq ... both could happen, but ignore it either way */
					linklist_iter_del( global_recvq_start );
					
					if (temp->recvq->members > 0)
					{
						/* Must be dead but with a recvq still ... decrease the recvq
						 * don't worry about the size though, if it's dead then it can't get bigger
						 */
						
						/* HACK! NEED A FIFO LIST! */
						hack->list = temp->recvq;
						hack->current = NULL;
						line = linklist_iter_next( hack );
						linklist_iter_del( hack );
						free(line);
					}
					/* Since we didn't do much with this one, we'll try another pass... */
				}
			}
		}
		
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
