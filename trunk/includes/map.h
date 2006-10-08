/*
 * Stuff for map.c
 */

#ifndef __map_include__
#define __map_include__

/*
 * Typedef section
 */
typedef struct map_root map_root;
typedef struct map_branch map_branch;
typedef struct map_iter map_iter;

/*
 * Now for some structs
 */
struct map_branch
{
	map_branch *parent;
	unsigned char *match;
	unsigned char *key;
	void *data;
	int children;
	map_branch **child;
};

struct map_root
{
	int state;
	int mapping[256];
	int valid_char_count;
	map_branch root;
	slinklist_root *sll;
};

struct map_iter
{
	map_root *root;
	slinklist_iter *iter;
};

/*
 * Prototype!
 */
extern map_root *map_create();
extern void map_allow_char(map_root *root, unsigned char c);
extern void map_disallow_char(map_root *root, unsigned char c);
extern void map_letter_alias(map_root *root, unsigned char alias, unsigned char ptr_to);
extern void map_init(map_root *root);

extern int map_add(map_root *root, unsigned char *match, void *data);
extern int map_validate_key(map_root *root, unsigned char *key);
extern int map_del(map_root *root, unsigned char *match);
extern void *map_find(map_root *, unsigned char *);
extern int map_free(map_root *root);

extern map_iter *map_iter_create(map_root *root);
extern void *map_iter_next(map_iter *iter);
extern void map_iter_del(map_iter *iter);
extern void *map_iter_prev(map_iter *iter);
extern void map_iter_seek_start(map_iter *iter);
#define map_iter_free(x) free(x)

extern void map_allow_preset(map_root *root, const int preset[257]);
extern map_root *map_create_from_preset(const int preset[257]);
extern map_root *map_create_init_from_preset(const int preset[257]);

/********************************************************
 * This section is special ...													*
 * these externs are for the map_preset() function.			*
 * There actual definitions are in map.c								*
 ********************************************************/
 
extern const int MAP_ALLOW_ALL[257];
extern const int MAP_ALLOW_AZ[257];
extern const int MAP_ALLOW_AZ_CI[257];
extern const int MAP_ALLOW_AZ_SPACE[257];
extern const int MAP_ALLOW_AZ_SPACE_CI[257];
extern const int MAP_ALLOW_AZ09[257];
extern const int MAP_ALLOW_AZ09_CI[257];
extern const int MAP_ALLOW_AZ09_SPACE[257];
extern const int MAP_ALLOW_AZ09_SPACE_CI[257];

extern const int ALLOWED_NICK_CHARS[257];
extern const int ALLOWED_CHAN_CHARS[257];

#endif
