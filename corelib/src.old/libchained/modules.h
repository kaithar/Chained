/**
 * @file modules.h
 * @todo Document this file too!
 */

#ifndef _INCLUDES_MODULES_H_
#define _INCLUDES_MODULES_H_

extern void cis_modules_set_path (unsigned char *path);
extern module *cis_module_load(char *modfile);
extern bool cis_module_unload(module *m);

#endif
