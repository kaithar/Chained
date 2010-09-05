/* 
 * File:   externs.h
 * Author: nazca
 *
 * Created on May 30, 2010, 10:06 PM
 */

#ifndef _EXTERNS_H
#define	_EXTERNS_H

#ifdef	__cplusplus
extern "C" {
#endif

extern reactor_config r_config;
extern connection *connections[__MAXFDS__];

#ifdef	__cplusplus
}
#endif

#endif	/* _EXTERNS_H */

