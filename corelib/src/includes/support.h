#ifndef _INCLUDES_SUPPORT_H_
#define _INCLUDES_SUPPORT_H_

extern void *smalloc(size_t size);
extern void sfree(void *);
extern int cis_do_blocking_exec (unsigned char *file, unsigned char *buffer, unsigned int n);

#endif
