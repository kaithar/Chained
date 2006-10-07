/**
** @file main.c
** @brief Main setup stuff.
** Chained in Sanity.
** Core Library.
** This is where the main core stuff happens
**/

#include "chained.h"
#include <sys/resource.h> /* For rlimit stuff */

/**
 * @BRIEF I am set by config param "debug"
 * @INTERNAL
 * @TODO This should probably be adjusted to a bitmask or something
 */

int DEBUG = 0; // Set by the config keyword "debug" ... no params

/**
 * @BRIEF I am set by the socket engine of your choice
 * @INTERNAL
 */
socket_engine *socketengine = NULL;

/**
 * @BRIEF Connection tracking...
 * @INTERNAL
 */
connection *connections[__MAXFDS__];

/**
 * @BRIEF I try to make sure we're dropping cores...
 * This function attempts to make sure we drop cores.
 * Should it fail, it will generate an error to the stderr.
 * @TODO
 * Ideally, we need some way of making error here fatal.
 * That way we can get the program to halt if it won't be able to drop cores.
 * @NOTE
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
			sprintf(stderr,"setting the resource limit failed with errno: %d\n",errno);
		}
	}
	return;
}

/**
 * @BRIEF I initialize the various componants to make them ready for use.
 * This function should be called very early on, as it make the library ready for use.
 * Amongst other things it calls srand and mallocs some stuff.
 */

void cis_init (void)
{
	static initd = 0;
	
	if (initd == 1)
		sprintf(stderr,"You attempted to initalize the library twice!\n");
	
	srand(time(0));
	memset(connections,0,sizeof(connections));
	
	initd = 1;
}

/**
 * @BRIEF I make the magic happen... I am the main loop!
 * This function contains the main "reactor" loop...\n
 * The loop that makes it all happen basically.
 * The following things get called from this function:
 * -Socket engine
 * -Timer loop
 * -Connection Reaper
 * @TODO
 * -#Although we can listen for write on a socket, the socket loop doesn't do anything with it.
 * -#Infact, the whole socket loop should probably be slimed down!  This function needs to be simple! 
 */

void cis_start (void)
{
	int i = 0;
	int eventcount = 0;
	connection *read_events[__MAXFDS__];
	connection *write_events[__MAXFDS__];
	
	/* main loop */
	for(;;) {
		
		eventcount = socketengine->wait(read_events, write_events);
		
		/* Run through the existing connections looking for data to read */
		for(i = 0; i < eventcount; i++) {
			if (read_events[i] != NULL)
			{
				if (read_events[i]->doRead(read_events[i]) < 0) {
					if (read_events[i]->doClose)
						read_events[i]->doClose(read_events[i]); // bye!
					else
					{
						sprintf(stderr,"Object registered without a close!\nAttempting emergency close!\n");
						close(read_events[i]->fd);
					}
				}
			}
		} // foreach (event)
		processTimers();
	}
	return 0;
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