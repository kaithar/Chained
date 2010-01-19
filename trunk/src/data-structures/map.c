/*
 * Chained
 *
 * Map management.
 *
 * Note: We don't technically provide hash tables ... we provide map thingys
 */


#include "libchained/chained.h"

#define MAP_STATE_SETUP 0
#define MAP_STATE_NORMAL 1

/************************************************
 * Map functions,                               *
 * Not a hash map, just a plain map             *
 * Depends on slinklist heavily!                *
 ************************************************/

/*
 * Support function: Test string equivelence.
 * Since maps can define their own character equivelents, 
 * this function is needed to test to see if two strings match.
 */
static int map_strcmp(map_root *root, unsigned char *str1, unsigned char *str2)
{
	int d = 0;
	unsigned char *c1 = str1;
	unsigned char *c2 = str2;

	while (1)
	{
		if ((*c1 == '\0') && (*c2 == '\0'))
			return 0;

		d = root->mapping[*c1] - root->mapping[*c2];
		
		if ((d < 0) || (*c1 == '\0'))
			return -1;
		
		if ((d > 1) || (*c2 == '\0'))
			return 1;
		
		c1++;
		c2++;
	}
}

/* Test for key validity */
int map_validate_key(map_root *root, unsigned char *key)
{
	unsigned char *c = key;

	while (1)
	{
		if (*c == '\0')
			return true;
	
		if (root->mapping[*c] < 0)
			return false;
		
		c++;
	}
}

/*
 * Create map structure!
 */
map_root *map_create()
{
	map_root *temp = NULL;
	temp = smalloc(sizeof(map_root));

	return temp;
}

/*
 * Functions to manipulate the allowed characters
 */
void map_allow_char(map_root *root, unsigned char c)
{
	if ((root == NULL) || (root->state != MAP_STATE_SETUP))
		abort();
	
	if (root->mapping[c] != -747)
		root->valid_char_count++;
	
	root->mapping[c] = -747;
}

void map_disallow_char(map_root *root, unsigned char c)
{
	if ((root == NULL) || (root->state != MAP_STATE_SETUP))
		abort();

	if (root->mapping[c] == -747)
		root->valid_char_count--;
	
	root->mapping[c] = 0;
}

void map_letter_alias(map_root *root, unsigned char alias, unsigned char ptr_to)
{
	if ((root == NULL) || (root->state != MAP_STATE_SETUP))
		abort();

	if (root->mapping[alias] == -747)
		root->valid_char_count--;
	
	root->mapping[alias] = -1000 - ptr_to;
}

/*
 * Mappings!
 * These are the presets...
 * 0x100 (256) is the number of valid chars, the rest of the array is said characters...
 */
void map_allow_preset(map_root *root, const int preset[257])
{
	int c = 0;

	for (c = 0; c <= 255; c++)
		root->mapping[c] = preset[c];
	
	root->valid_char_count = preset[0x100];
}

/*
 * Save some calls...
 */
map_root *map_create_from_preset(const int preset[257])
{
	map_root *temp = NULL;
	temp = map_create();
	map_allow_preset(temp, preset);
	
	return temp;
}

map_root *map_create_init_from_preset(const int preset[257])
{
	map_root *temp = NULL;
	temp = map_create();
	map_allow_preset(temp, preset);
	map_init(temp);
	
	return temp;
}

#include "libchained/map_presets.h"

/*
 * Finalise the struct (called init for purposes of avoiding z/s complaints)
 */
void map_init(map_root *root)
{
	int i = 0;
	int j = 0;

	if ((root == NULL) || (root->valid_char_count == 0) || (root->state != MAP_STATE_SETUP))
		abort();

	for (i = 0; i <= 255; i++)
	{
		if (root->mapping[i] == -747)
		{
			/* Assign this one a mapping */
			root->mapping[i] = j++;
		}
		else if (root->mapping[i] == 0)
		{
			/* Negate it. */
			root->mapping[i] = -1;
		}
	}
	
	for (i = 0; i <= 255; i++)
	{
		if ((root->mapping[i] <= -1000) && (root->mapping[i] >= -1255))
		{
			j = root->mapping[i];
	
			while ((j <= -1000) && (root->mapping[i] >= -1255))
			{
				if (j == -(i + 1000))
					abort();
			
				j = root->mapping[-(j + 1000)];
			}

			if ((j < 0) || (j > 255))
				root->mapping[i] = -1;
			else
				root->mapping[i] = j;
		}
	}
	root->root.child = (map_branch **) smalloc(root->valid_char_count * sizeof(map_branch *));
	root->sll = slinklist_create();
	root->state = MAP_STATE_NORMAL;
}

/*
 * Insert ...
 */
static int map_branch_descend(map_root *root, map_branch *location)
{
	/* The principle behind this function is to move a branch down a nest level with the least work possible */
	map_branch *temp = NULL;
	temp = smalloc(sizeof(map_branch));
	temp->child = (map_branch **) smalloc(root->valid_char_count * sizeof(map_branch *));
	temp->data = location->data;
	temp->key = location->key;
	temp->match = location->match + 1;
	temp->parent = location;
	location->child[root->mapping[*location->match]] = temp;
	location->data = NULL;
	location->key = NULL;
	location->match = NULL;
	location->children = 1;

	return 0; /* XXX: make this void? */
}

void map_print_r(map_root *root, unsigned char *rmap, unsigned char *foo, unsigned char *c, map_branch *location)
{
	/* Recursive for below... */
	int i;

	if (location->match != NULL)
	{
		*c = '\0';
		printf("%s|%s: %s\n", foo, location->match, location->key);
		/*printf("%s|%s: %s %d\n", foo, location->match, location->key, location->data); // XXX: what is data supposed to return? */
	}
	
	for (i = 0; i < root->valid_char_count; i++)
	{
		if (location->child[i] != NULL)
		{
			*c = rmap[i];
			map_print_r(root, rmap, foo, c + 1, location->child[i]);
		}
	}
}

void map_print(map_root *root)
{
	/* For debug purposes, this prints the content of a map */
	unsigned char foo[1000];
	unsigned char *c = foo;
	unsigned char rmap[256];
	int i;

	slinklist_iter *temp = NULL;
	char *bar = NULL;

	for (i = 0; i < 256; i++)
	{
		if (root->mapping[i] >= 0)
		{
			rmap[root->mapping[i]] = i;
		}
	}

	map_print_r(root, rmap, foo, c, &root->root);
	 /**/ temp = slinklist_iter_create(root->sll);

	printf("F : ");

	while ((bar = slinklist_iter_next(temp)) != NULL)
		printf("%s > ", temp->current->key);

	printf("\nR : ");

	while ((bar = slinklist_iter_prev(temp)) != NULL)
		printf("%s > ", temp->current->key);

	printf("\nEOL\n\n");
	slinklist_iter_free(temp);
}

/*
 * Simple logic here is as thus...
 * starting at root, proceed through the branches til no other string match is possible
 * then insert the new one at that node...
 */
int map_add(map_root *root, unsigned char *match, void *data)
{
	int len = 0;
	unsigned char *ch = match;
	map_branch *temp = NULL;
	map_branch *location = &root->root;

	if ((root == NULL) || (root->state != MAP_STATE_NORMAL) || (match == NULL) || (data == NULL))
		abort();

	/* Test key for validity ... */
	for (; *ch != '\0'; ch++)
		if (root->mapping[*ch] < 0)
			return false;

	ch = match;

	/* Drill down as far as possible... */
	while (1)
	{
		if (*ch == '\0')	/* Out of key, is there one here already? */
		{
			if (location->match != NULL)
			{
				if (*location->match == '\0')
				{	/* Fatal collision! */
					return false;
				}

				/* Descend it first... */
				map_branch_descend(root, location);
			}
			/* And now we break. */
			break;
		}

		if (location->child[root->mapping[*ch]] == NULL)	/* Out of drill down */
		{
			if (location->match != NULL)
			{
				/* Split! */
				if (location->match[0] == *ch)
				{	/* Possible delayed split, but test for a collision first. */
					if (map_strcmp(root, location->match, ch) == 0)
					{	/* EEK! Collision! */
						return false;
					}
		
					/* Delayed split ... descend and continue; */
					map_branch_descend(root, location);
					location = location->child[root->mapping[*ch]];
					ch++;
					
					continue;
				}
				/* Perhaps it's a substring of the one we're processing now (ie, leave it where it is)? */
				if (*location->match == '\0')
				{
					break;
				}
				else
				{
					/* Normal split, descend and break */
					map_branch_descend(root, location);
					
					break;
				}
			}

			/* Weird, No drill down, no match, might be a child elsewhere ... break time */
			break;
		}
		/* Drill down! */
		location = location->child[root->mapping[*ch]];
		ch++;
	}

	/*
	 * Okie, location is now at the node we wanna insert at ... 
	 * (unless we are at the end of the string, in which case we insert at location, instead of below it)

	 * sooo...

	 * End of match?
	 */
	if (*ch == '\0')
	{			/* End of match ... */
		location->key = location->match = smalloc(sizeof(char) * 1);
		location->key[0] = '\0';
		location->data = data;
	}
	else
	{
		/* Need to assign a branch here... */
		location->child[root->mapping[*ch]] = temp = smalloc(sizeof(map_branch));
		location->children++;
		temp->child = (map_branch **) smalloc(root->valid_char_count * sizeof(map_branch *));
		len = strlen((char *)match);
		temp->key = (unsigned char *)strdup((char *)match);
		temp->match = temp->key + ((ch + 1) - match);	/* ((ch+1) - match) should be the offset. */
		temp->data = data;
		temp->parent = location;
	}

	/* Slinklist add! */
	slinklist_add(root->sll, match, data);
	
	return true;
}


int map_del(map_root *root, unsigned char *match)
{
	/* This works on a similar thought to add ...  */
	unsigned char *ch = match;
	map_branch *location = &root->root;

	if ((root == NULL) || (root->state != MAP_STATE_NORMAL) || (match == NULL))
		abort();

	/* Test key for validity ... can't remove a key that isn't valid ;) */
	for (; *ch != '\0'; ch++)
		if (root->mapping[*ch] < 0)
			return 0;

	ch = match;

	/* Drill down as far as possible... */
	while (1)
	{
		if (*ch == '\0')	/* Out of key, is there one here already? */
		{
			if ((location->match != NULL) && (*location->match == '\0'))
			{	/* Match! */
				break;
			}
			/* Out of key, but no match ... ergo no key to remove */
			return 0;
		}
		if (location->child[root->mapping[*ch]] == NULL)	/* Out of drill down */
		{
			if (location->match == NULL)
				return 0;	/* Out of ground and nobody's home. */

			/* Does the current key match the targeted key? */
			if (map_strcmp(root, location->match, ch) == 0)
				break;	/* Yup! */

			/* Seems not ... thats no match then */
			return 0;
		}
		/* Drill down! */
		location = location->child[root->mapping[*ch]];
		ch++;
	}

	/* Okie, location is the node we need to nix. */
	free(location->key);
	location->key = NULL;
	location->match = NULL;

	while ((location->children == 0) && (location->parent != NULL) && (location->match == NULL))
	{
		ch--;
		free(location->child);
		location = location->parent;
		location->children--;
		free(location->child[root->mapping[*ch]]);
		location->child[root->mapping[*ch]] = NULL;
	}

	/* Slinklist del! */
	slinklist_del_key(root->sll, match);

	return 1;
}

void *map_find(map_root *root, unsigned char *match)
{
	/* Yup, you guessed it, same basic idea as adding and deleting */
	unsigned char *ch = match;
	map_branch *location = &root->root;

	if ((root == NULL) || (root->state != MAP_STATE_NORMAL) || (match == NULL))
		abort();

	/* Test key for validity ... can't find a key that isn't valid ;) */
	for (; *ch != '\0'; ch++)
		if (root->mapping[*ch] < 0)
			return NULL;

	ch = match;

	/* Drill down as far as possible... */
	while (1)
	{
		if (*ch == '\0')	/* Out of key, is there one here already? */
		{
			if ((location->match != NULL) && (*location->match == '\0'))
			{	/* Match! */
				return location->data;
			}
	
			/* Out of key, but no match ... ergo no key to remove */
			return NULL;
		}
		if (location->child[root->mapping[*ch]] == NULL)	/* Out of drill down */
		{
			if (location->match == NULL)
				return NULL;	/* Out of ground and nobody's home. */

			/* Does the current key match the targeted key? */
			if (map_strcmp(root, location->match, ch) == 0)
				return location->data;	/* Yup! */

			/* Seems not ... thats no match then */
			return NULL;
		}
		/* Drill down! */
		location = location->child[root->mapping[*ch]];
		ch++;
	}

	return NULL;
}

int map_free(map_root *root)
{
	/* Okie, now -this- is different :P */
	slinklist_iter *temp = NULL;
	map_branch *location = NULL;
	unsigned char *ch = NULL;
	
	if (root == NULL)
		abort();
	
	if (root->state == MAP_STATE_NORMAL)
	{
		temp = slinklist_iter_create(root->sll);
	
		while (1)
		{
			slinklist_iter_next(temp);

			if (temp->current == NULL)
				break;
		
			ch = temp->current->key;
			location = &root->root;

			/* Drill down as far as possible... */
			while (1)
			{
				if (*ch == '\0')	/* Out of key, is there one here? */
				{
					if ((location->match != NULL) && (*location->match == '\0'))
					{	/* Match! */
						break;
					}
			
					/* Out of key, but no match ... ergo no key to remove */
					abort();
				}
				
				if (location->child[root->mapping[*ch]] == NULL)	/* Out of drill down */
				{
					if (location->match == NULL)
						abort();	/* Out of ground and nobody's home. */

					/* Does the current key match the targeted key? */
					if (map_strcmp(root, location->match, ch) == 0)
						break;	/* Yup! */

					/* Seems not ... thats no match then */
					abort();
				}

				/* Drill down! */
				location = location->child[root->mapping[*ch]];
				ch++;
			}
			/*
			 * Start killing!
			 * Okie, kill the key, then starting at location:
			 * Go up a level, free, decrease children, repeat
			 * if there is no parent, we assume we're at the root...
			 * if there are children we can't free or we'll corrupt the map.
			 *
			 * This assumption -should- be suffcient, but could do with being tested.
			 */
			free(location->key);
			
			while (1)
			{
				ch--;
			
				if ((location->parent != NULL) && (location->children == 0))
				{
					free(location->child);
					location = location->parent;
					free(location->child[root->mapping[*ch]]);
					location->children--;
				}
				else
				{	/* This doesn't have a parent, or had more children... */
					break;
				}
			}

			if ((location->children == 0) && (location->parent == NULL))
			{
				/*
				 * Assumably this means we've free'd everything else
				 * again, this assumption will need some testing
				 * At this point (when everything is free), we wanna break out and nuke the root
				 */
				break;
			}
		}

		slinklist_iter_free(temp);
		slinklist_free(root->sll);
		free(root->root.child);
	}

	free(root);
	
	return true;
}

map_iter *map_iter_create(map_root *root)
{
	map_iter *temp = NULL;

	if ((root == NULL) || (root->state != MAP_STATE_NORMAL))
		abort();
	
	temp = smalloc(sizeof(slinklist_iter));
	temp->root = root;
	temp->iter = slinklist_iter_create(root->sll);
	
	return temp;
}

void *map_iter_free(map_iter *iter)
{
	slinklist_iter_free(iter->iter);
	free(iter);
}

void *map_iter_next(map_iter *iter)
{
	if (iter == NULL)
		abort();

	return slinklist_iter_next(iter->iter);
}

void map_iter_del(map_iter *iter)
{
	unsigned char *temp;
	
	if (iter == NULL)
		abort();
	
	temp = iter->iter->current->key;
	slinklist_iter_prev(iter->iter);
	map_del(iter->root, temp);
	
	return;
}

void *map_iter_prev(map_iter *iter)
{
	if (iter == NULL)
		abort();
	
	return slinklist_iter_prev(iter->iter);
}

void map_iter_seek_start(map_iter *iter)
{
	if (iter == NULL)
		abort();
	
	slinklist_iter_seek_start(iter->iter);
}
