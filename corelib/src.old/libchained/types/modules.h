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

	void *handle;
};

struct moduleheader
{
	unsigned long compat;
	long api_ver;
	char *name;
	bool(*modinit)(module *);
	void (*deinit)();
  bool (*modconfig)(module *, cis_config_node *);
	char *version;
	char *vendor;
};

#define MAPI_MAGIC	0xBEEFCAFE
#define MAPI_V		2

#define DECLARE_MODULE_V1(name, modinit, deinit, ver, ven) \
	moduleheader _header = {				\
		MAPI_MAGIC, MAPI_V,				\
		name, modinit, deinit, NULL, ver, ven		\
}

#define DECLARE_MODULE_V2(name, modinit, deinit, modconfig, ver, ven) \
  moduleheader _header = {        \
    MAPI_MAGIC, MAPI_V,        \
    name, modinit, deinit, modconfig, ver, ven   \
}

#endif
