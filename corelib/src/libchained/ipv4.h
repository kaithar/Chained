/**
 * @file ipv4.h
 * @brief IPv4 functions.
 * Chained in Sanity
 * IPv4
 * This file contains the various ipv4 function prototypes
 */

#ifndef _INCLUDES_IPV4_H_
#define _INCLUDES_IPV4_H_

extern connection *ipv4_tcp_listen(char* name, int ip);
extern connection *ipv4_tcp_connect (char* stream_name, int target_host);
extern int ipv4_tcp_read(connection *, int how_much, char *buffer);
extern int ipv4_tcp_write(connection *stream, char *str);
extern int ipv4_tcp_close(connection *stream);

#endif
