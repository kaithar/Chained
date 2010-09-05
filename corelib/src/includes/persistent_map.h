/**
 * @file persistent_map.h
 * Chained in Sanity
 */

#ifndef _INCLUDES_PERSISTENT_MAP_H_
#define _INCLUDES_PERSISTENT_MAP_H_

typedef struct pm_root pm_root;
typedef struct pm_node pm_node;
typedef struct pm_iter pm_iter;

struct pm_root
{
	char *filename;
	map_root *map;
};

struct pm_node
{
	char *key;
	char *type_C;
};

struct pm_iter
{
	map_iter *iter;
	pm_root *pm;
};

extern pm_root *cis_pm_create (char *filename);
extern int cis_pm_load (pm_root *root);
extern int cis_pm_set_char(pm_root *root, char *key, char *value);
extern int cis_pm_del(pm_root *root, char *key);
extern int cis_pm_store(pm_root *root);

extern pm_iter *cis_pm_iter_create(pm_root *root);
extern char *cis_pm_iter_next_char(pm_iter *iter);
extern char *cis_pm_iter_prev_char(pm_iter *iter);
extern void cis_pm_iter_del(pm_iter *iter);
extern void cis_pm_iter_free(pm_iter *iter);
extern void cis_pm_iter_seek_start(pm_iter *iter);
#endif
