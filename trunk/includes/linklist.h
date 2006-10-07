/*
 * Stuff for linklist.c
 */

#ifndef __linklist_include__
#define __linklist_include__

/*
 * Typedef section
 */
typedef struct linklist_root linklist_root;
typedef struct linklist_link linklist_link;
typedef struct linklist_iter linklist_iter;

typedef struct slinklist_root slinklist_root;
typedef struct slinklist_link slinklist_link;
typedef struct slinklist_iter slinklist_iter;

/*
 * Now for some structs
 */
struct linklist_root
{
	linklist_link *first, *last;
	int members;
};

struct linklist_link
{
	linklist_link *next, *prev;
	linklist_root *list;
	void *data;
};

struct linklist_iter
{
	linklist_link *current;
	linklist_root *list;
};

/* --- */
struct slinklist_root
{
	slinklist_link *first, *last;
	slinklist_link *index[256];
	int members;
};

struct slinklist_link
{
	slinklist_link *next, *prev;
	slinklist_root *list;
	unsigned char *key;
	void *data;
};

struct slinklist_iter
{
	slinklist_link *current;
	slinklist_root *list;
};

/*
 * Prototype!
 */
extern linklist_root *linklist_create();
extern void linklist_add(linklist_root *parent, void *data);
extern int linklist_del(linklist_root *list, void *data);
extern int linklist_free(linklist_root *list);

extern linklist_iter *linklist_iter_create(linklist_root *list);
extern void *linklist_iter_next(linklist_iter *iter);
extern void *linklist_iter_prev(linklist_iter *iter);
extern void linklist_iter_seek_start(linklist_iter *iter);
extern void linklist_iter_del(linklist_iter *iter);
#define linklist_iter_free(x) free(x)

/* --- */
extern slinklist_root *slinklist_create();
extern bool slinklist_add(slinklist_root *, unsigned char *, void *);
extern int slinklist_del_key(slinklist_root *, unsigned char *);
extern int slinklist_del_value(slinklist_root *, void *);
extern int slinklist_free(slinklist_root *);

extern slinklist_iter *slinklist_iter_create(slinklist_root *);
extern void *slinklist_iter_next(slinklist_iter *);
extern void *slinklist_iter_prev(slinklist_iter *);
extern void slinklist_iter_seek_start(slinklist_iter *);
extern void slinklist_iter_del(slinklist_iter *);
#define slinklist_iter_free(x) free(x)

#endif
