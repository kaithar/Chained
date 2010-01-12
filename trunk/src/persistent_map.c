/*
 * Chained
 *
 * Persistent Map Management.
 *
 * Currently this might be better named Persistent String Storage,
 * but it is designed to be expandable to other types should one be
 * inclined to implement wrappers for them.
 */

#include "libchained/chained.h"

pm_root *cis_pm_create (char *filename)
{
	pm_root *temp;

	if (filename == NULL)
		abort();

	temp = smalloc(sizeof(pm_root));
	temp->map = map_create_init_from_preset(MAP_ALLOW_PM_KEYS);
	temp->filename = strdup(filename);
	cis_pm_load(temp);
	return temp;
}

int cis_pm_load (pm_root *root)
{
	FILE *foo;
	static char buf[5001];
	int len = 0;
	char *key = NULL;
	char *value = NULL;
	int len2 = 0;
	char *c = buf;
	int count = 0;
	int action = 0;
	char type = 'C';
	pm_node *node = NULL;

	foo = fopen(root->filename, "r");
	if (foo == NULL)
	{
		fprintf(stderr,"cis_pm_load: Unable to open %s for reading.\n",root->filename);
		return 0;
	}
	
	if (!((getc(foo) == 'V') && (getc(foo) == '1') && (getc(foo) == '\n')))
	{
		fprintf(stderr,"cis_pm_load: Incompatible file version in %s.\n",root->filename);
		fclose(foo);
		return 0;
	}

	memset(buf,0,5001);

	for (;;)
	{
		key = value = NULL;
		c = buf;
		action = 0;
		len = len2 = 0;
		/* Read key... */
		for (;;len++)
		{
			if (len == 5000)
			{
				if ((key = realloc(key, len2 + 5000 + 1)) == NULL) abort();
				memcpy(key + len2, buf, 5000);
				len2 += 5000;
				key[len2] = '\0';
				c = buf;
				len = 0;
			}

			*c = getc(foo);
			switch (*c)
			{
				case EOF:
					fclose(foo);
					if (key != NULL) free(key);
					return count;
				case '\n':
				case '\r':
					if (key != NULL) free(key);
					action = 2;
					break;
				case ' ':
					*c = '\0';
					action = 1;
					break;
				default:
					c++;
			}
			if (action > 0)	break;
		}
		if (action == 2) continue;

		if (len > 0)
		{
			if ((key = realloc(key, len2 + len + 1)) == NULL) abort();
			memcpy(key + len2, buf, len);
			len2 += len;
			key[len2] = '\0';
		}

		if (len2 == 0)
		{
			continue;
		}

		switch (type = getc(foo))
		{
			case 'C':
				break;
			default:
				fprintf(stderr,"cis_pm_load: Unknown key type %c for %s in %s.\n",type, key, root->filename);
				free(key);
				fclose(foo);
				abort(); /* Is there an arguement for an alternative behaviour here? */
		}

		if (getc(foo) != ' ')
		{
			fprintf(stderr,"cis_pm_load: Bad format for %s in %s.\n", key, root->filename);
				free(key);
				fclose(foo);
				abort(); /* Is there an arguement for an alternative behaviour here? */
		}

		c = buf;
		len = len2 = 0;
		action = 0;
		/* Read value... */
		for (;;len++)
		{
			if (len == 5000)
			{
				if ((key = realloc(value, len2 + 5000 + 1)) == NULL) abort();
				memcpy(value + len2, buf, 5000);
				len2 += 5000;
				value[len2] = '\0';
				c = buf;
				len = 0;
			}

			*c = getc(foo);
			switch (*c)
			{
				case EOF:
					*c = '\0';
					action = 2;
					break;
				case '\n':
				case '\r':
					action = 1;
					break;
				case '\\':
					c[1] = getc(foo);
					switch (c[1])
					{
						case EOF:
							*c = '\0';
							action = 2;
							break;
						case 'n': *(c++) = '\n'; break;
						case 'r': *(c++) = '\r'; break;
						case '\\': *(c++) = '\\'; break;
						default: c += 2;
					}
					break;
				default:
					c++;
			}
			if (action > 0)	break;
		}

		if (len > 0)
		{
			if ((value = realloc(value, len2 + len + 1)) == NULL) abort();
			memcpy(value + len2, buf, len);
			len2 += len;
			value[len2] = '\0';
		}

		if (len2 == 0)
		{
			free(key);
			if (action == 2) break;
			else continue;
		}

		node = smalloc(sizeof(node));
		node->key = key;
		node->type_C = value;
		printf("%s C %s\n\n", key, value);
		map_add(root->map, key, node);
		count++;
		if (action == 2) break;
	}
  return count;
}

/** This function is blocking by design.  You have been warned. */
int cis_pm_store(pm_root *root)
{
	int fd = 0;
	FILE *foo;
	char t[] = "cis_pm.XXXXXX";
	char *c = NULL;
	int count = 0;
	map_iter *iter;
	pm_node *node = NULL;
	
	fd = mkstemp(t);
	foo = fdopen(fd, "w");
	if (foo == NULL)
	{
		fprintf(stderr,"cis_pm_store: Unable to open temp file for writing.\n");
		return -1;
	}
	
	if (fputs("V1\n", foo) == EOF)
	{
		fprintf(stderr,"cis_pm_store: Unable to write to temp file.\n");
		return -1;
	}

	iter = map_iter_create(root->map);
	
	while ((node = map_iter_next(iter)) != NULL)
	{
		printf("%s C %s\n\n", node->key, node->type_C);
		if (fputs(node->key, foo) == EOF) { fprintf(stderr, "cis_pm_store: Write error.\n"); return -1; }
		if (fputs(" C ", foo) == EOF) { fprintf(stderr, "cis_pm_store: Write error.\n"); return -1; }
		for (c = node->type_C; *c != '\0'; c++)
		{
			switch (*c)
			{
				case '\n':
					if (fputs("\\n", foo) == EOF)
					{
						fprintf(stderr, "cis_pm_store: Write error.\n"); 
						return -1;
					}
					break;
					
				case '\r':
					if (fputs("\\r", foo) == EOF)
					{
						fprintf(stderr, "cis_pm_store: Write error.\n");
						return -1;
					}
					break;
					
				case '\\':
					if (fputs("\\", foo) == EOF)
					{
						fprintf(stderr, "cis_pm_store: Write error.\n");
						return -1;
					}
					break;
					
				default:
					if (fputc(*c, foo) == EOF)
					{
						fprintf(stderr, "cis_pm_store: Write error.\n");
						return -1;
					}
					break;
			}
		}
		if (fputc('\n', foo) == EOF)
		{
			fprintf(stderr, "cis_pm_store: Write error.\n");
			return -1;
		}
		count++;
	}
	fclose(foo);
	close(fd);
	rename(t, root->filename);
  return count;
}

int cis_pm_set_char(pm_root *root, char *key, char *value)
{
	pm_node *node = NULL;
	if ((root == NULL) || (key == NULL) || (value == NULL))
		abort();

	if ((node = map_find(root->map, key)) == NULL)
	{
		node = smalloc(sizeof(node));
		node->key = strdup(key);
		node->type_C = strdup(value);
		printf("%s C %s\n\n", key, value);
		if (map_add(root->map, key, node) == false) return false;
	}
	else
	{
		if (node->type_C != NULL) free(node->type_C);
		node->type_C = strdup(value);
		printf("%s C %s (prev value overwritten)\n\n", key, value);
	}
	cis_pm_store(root);
	return true;
}

int cis_pm_del(pm_root *root, char *key)
{
	pm_node *node = NULL;

	if ((root == NULL) || (key == NULL))
		abort();

	node = map_find(root->map, key);

	if (node == NULL)
		return 0;

	free(node->key);
	if (node->type_C) free(node->type_C);
	map_del(root->map, key);
	free(node);

	cis_pm_store(root);

	return 1;
}

pm_iter *cis_pm_iter_create(pm_root *root)
{
	pm_iter *temp;
	if (root == NULL)
		abort();
	temp = smalloc(sizeof(pm_iter));
	temp->pm = root;
	temp->iter = map_iter_create(root->map);
	return temp;
}

char *cis_pm_iter_next_char(pm_iter *iter)
{
	pm_node *temp;

	if (iter == NULL)
		abort();

	while ((temp = map_iter_next(iter->iter)) != NULL)
	{
		if (temp->type_C != NULL)
			return temp->type_C;
	}
	return NULL;
}

char *cis_pm_iter_prev_char(pm_iter *iter)
{
	pm_node *temp;

	if (iter == NULL)
		abort();

	while ((temp = map_iter_prev(iter->iter)) != NULL)
	{
		if (temp->type_C != NULL)
			return temp->type_C;
	}
	return NULL;
}

void cis_pm_iter_del(pm_iter *iter)
{
	pm_node *node;

	if (iter == NULL)
		abort();

	node = iter->iter->iter->current->data;
	if (node == NULL)
		return;

	map_iter_prev(iter->iter);
	map_del(iter->pm->map, node->key);

	free(node->key);
	if (node->type_C) free(node->type_C);
	free(node);

	cis_pm_store(iter->pm);
}

void cis_pm_iter_free(pm_iter *iter)
{
	map_iter_free(iter->iter);
	free(iter);
}

void cis_pm_iter_seek_start(pm_iter *iter)
{
	map_iter_seek_start(iter->iter);
}
