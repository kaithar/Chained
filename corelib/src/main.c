/**
 * @file main.c
 * @brief Main setup stuff.
 * Chained in Sanity.
 * Core Library.
 * This is where the main core stuff happens
 */

#include <includes/chained.h>

/* Internal prototypes */
//void cis_init_events();
//void cis_init_modules();


/**
 * @brief I initialize the various componants to make them ready for use.
 * This function should be called very early on, as it make the library ready for use.
 * Amongst other things it calls srand and mallocs some stuff.
 */

void cis_init (void)
{
	static char initd = 0;
	
	if (initd == 1)
		fprintf(stderr,"You attempted to initalize the library twice!\n");
	
	srand(time(0));
	memset(connections,0,sizeof(connections));
	
	//cis_init_events();
	//cis_init_modules();
	
	cis_init_reaper();
	
	initd = 1;
}

static bool reactor_running = false;

/**
 * @brief Kick out of the main loop
 * Use this if you want to stop the reactor
 */

void cis_kill_reactor (void)
{
	reactor_running = false;
}


/**
 * @brief I make the magic happen... I am the main loop!
 * This function contains the main "reactor" loop...\n
 * The loop that makes it all happen basically.
 * The following things get called from this function:
 * - Socket engine
 * - Readq processer
 * - Connection Reaper
 * - Timer loop
 */

void cis_run (void)
{
	r_config.patience = 250;
	r_config.global_recvq = fifo_create();

  /* main loop */
  for(reactor_running = true; reactor_running;)
  {
		cis_se_process();
		cis_readq_process();
		cis_reaper_process();
    //cis_timers_process();
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
