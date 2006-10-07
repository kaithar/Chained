/*
 * Function prototypes for main.h
 */

#ifndef _INCLUDES_MAIN_H_
#define _INCLUDES_MAIN_H_

extern int DEBUG;
extern socket_engine *socketengine;

extern void cis_drop_cores (void);
extern void cis_init (void);
extern void cis_start (void);

#endif
