/**
 * @file ssl.h
 * Chained in Sanity
 * This file contains information relating specifically to SSL support.
 */

#ifndef _INCLUDES_SSL_H_
#define _INCLUDES_SSL_H_

#ifdef __CIS_HAS_OPENSSL__
extern void cis_openssl_port_upgrade (connection *conn);
extern int cis_openssl_accept(connection *conn);
extern int cis_openssl_write(connection *stream, char *str);
extern int cis_openssl_read(connection *cn, int how_much, char *buffer);
extern int cis_openssl_close(connection *stream);

#endif
#endif
