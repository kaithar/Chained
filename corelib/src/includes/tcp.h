/**
 * @file tcp.h
 * @brief TCP functions.
 * Chained in Sanity
 * TCP
 * This file contains the various TCP function prototypes
 */

#ifndef _INCLUDES_TCP_H_
#define _INCLUDES_TCP_H_

extern connection *cis_tcp_listen(char *target_ip, int target_port);
extern connection *cis_tcp_connect (char *target_host, int target_port);
extern int cis_tcp_read(connection *);
extern int cis_tcp_write(connection *);
extern int cis_tcp_close(connection *);

#endif
