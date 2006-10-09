/*
 * Function prototypes for conn_basics.h
 */

#ifndef _INCLUDES_CONN_BASICS_H_
#define _INCLUDES_CONN_BASICS_H_

extern void conn_read_to_recvq (connection *cn);
extern bool conn_send_from_sendq (connection *cn);
extern int cprintf(connection *stream, char *fmt, ...);

#endif
