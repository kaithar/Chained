/*
 * Function prototypes for main.h
 */

#ifndef _INCLUDES_MAIN_H_
#define _INCLUDES_MAIN_H_

/* reactor/reaper.c */
extern void cis_reap_connection(connection *cn);
extern void cis_init_reaper();
extern void cis_reaper_process ( );

/* support/cores.c */
extern void cis_drop_cores (void);

/* main.c */
extern void cis_init (void);
extern void cis_run (void);
extern void cis_kill_reactor (void);

#endif
