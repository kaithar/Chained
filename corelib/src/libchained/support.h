#ifndef _INCLUDES_SUPPORT_H_
#define _INCLUDES_SUPPORT_H_

extern void  *smalloc(int size);
extern void sfree(void *);
extern int cis_do_blocking_exec (unsigned char *file, unsigned char *buffer, unsigned int n);
extern void discard_sendq (connection *conn);
extern void discard_recvq (connection *conn);

#endif
