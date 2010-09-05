/* 
 * File:   reactor_config.h
 * Author: nazca
 *
 * Created on May 30, 2010, 9:59 PM
 */

#ifndef _REACTOR_CONFIG_H
#define	_REACTOR_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct
{
	unsigned int patience;
	fifo_root *global_recvq;
	socket_engine *socketengine;
} reactor_config;

#ifdef	__cplusplus
}
#endif

#endif	/* _REACTOR_CONFIG_H */

