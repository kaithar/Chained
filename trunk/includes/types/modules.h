/**
 * @file modules.h
 * @todo Document this file!
 */

#ifndef _INCLUDES_TYPES_MODULES_H_
#define _INCLUDES_TYPES_MODULES_H_

typedef struct module module;
typedef struct moduleheader moduleheader;

struct module
{
	module *prev, *next;
	char *name;
	char *path;
	moduleheader *header;

	struct
	{
		unsigned int perm:1;
	} flags;

	void *address;
	void *handle;
};

struct moduleheader
{
	unsigned long compat;
	long api_ver;
	char *name;
	bool(*modinit)(module *);
	void (*deinit)();
	char *vendor;
	char *version;
};

#define MAPI_MAGIC	0xDEADBEEF
#define MAPI_V1		1

#define DECLARE_MODULE_V1(name, modinit, deinit, ver, ven) \
	moduleheader _header = {				\
		MAPI_MAGIC, MAPI_V1,				\
		name, modinit, deinit, ven, ver		\
}

#endif