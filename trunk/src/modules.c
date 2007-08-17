/**
 * Chained
 * Modules support!
 */

#include "chained.h"
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>		/* Dynamic loading stuff */

static linklist_root *modules = NULL;
static unsigned char *mod_path = NULL;
static int mod_path_length = 0;

void cis_init_modules()
{
	modules = linklist_create();
}

void cis_modules_set_path (unsigned char *path)
{
	unsigned char *c = path;
	if (path == NULL)
		abort();
	
	mod_path_length = strlen(path);
	
	while (*c != '\0')
		c++;
	c--;
	
	if (*c != '/')
		mod_path_length++;
	
	mod_path = smalloc(mod_path_length+1);
	snprintf(mod_path,mod_path_length+1,"%s",path);
	
	mod_path[mod_path_length] = '/';
	mod_path[mod_path_length+1] = '\0';
}

module *cis_module_load(char *modfile)
{
	module *m = smalloc(sizeof(module));
	moduleheader *h;
	int plen = mod_path_length + strlen(modfile);
	
	if (mod_path == NULL)
		abort();

	m->path = smalloc(plen);
	snprintf(m->path, plen, "%s%s", mod_path, modfile);
	m->handle = dlopen(m->path, RTLD_LAZY);

	if (!m->handle)
	{
		printf("Unable to open module %s: %s\n", modfile, dlerror());
		free(m->path);
		free(m);
		return NULL;
	}

	h = (moduleheader *) dlsym(m->handle, "_header");

	if (!h)
	{
		printf("Unable to find module header in %s: %s\n", modfile, dlerror());
		dlclose(m->handle);
		free(m->path);
		free(m);
		return NULL;
	}

	if (h->compat != MAPI_MAGIC)
	{
		printf("Incompatible module... please recompile %s\n", modfile);
		h = NULL;
		dlclose(m->handle);
		free(m->path);
		free(m);
		return NULL;
	}

	/* call module's init function */
	if (h->modinit)
	{
		if (h->modinit(m) == false)
		{
			/* load failed */
			printf("Module load returned FAIL\n");
			h = NULL;
			dlclose(m->handle);
			free(m->path);
			free(m);
			return NULL;
		}
	}
	
	m->header = h;

	linklist_add(modules,m);

	return m;
}

bool cis_module_unload(module *m)
{
	/* do nothing for now */
	return true;
}