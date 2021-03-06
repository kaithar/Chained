/**
 * @file config.c
 * @brief Config reader.
 * Chained in Sanity.
 * Core Library.
 * This file reads in a config.
 */

#include "libchained/chained.h"
#include <string.h>

static int cis_config_line = 0;
static unsigned char *cis_config_file = NULL;

static int getLineNum() { return cis_config_line; }
static unsigned char *getFileName() { return cis_config_file; }

static int singlelinecomment(FILE *configfile);
static int multilinecomment(FILE *configfile);

static void cis_config_include(FILE *configfile, cis_config_node *context);
static void cis_process_block (FILE *configfile, cis_config_node *context, bool included);

/**
 * Encapsulate getc() to simplify filtering out comments
 */

bool inquotes = false;
char quotechar = '\0';
int escaped = 0;
	
static char config_get_char (FILE *configfile, bool skip_whitespace, bool fatal_eof)
{	
	char c,c2;
	
	while (1)
	{
		c = getc(configfile);
		
		/* \ sets escaped to 2, the next char has escaped 1, the one after escaped 0 */
		if (--escaped < 0)
			escaped = 0;
		
		switch (c)
		{
			case EOF:
				if (!fatal_eof)
					return c;
				printf("Fatal EOF\n");
				exit(1);
				
			case '\\':
				if (escaped == 0)
					escaped = 2;
				return c;
				
			case '\n':
			case '\r':
				cis_config_line++;
			case ' ':
			case '\t':
				/**
				 * This case allows for skipping any whitespace it comes across
				 * This is especially usful for the begining of the line.
				 */
				if (skip_whitespace == true)
					break;
				else
					return c;
				
			case '"':
			case '\'':
				/**
				 * In addition to returning the quote, manage our internal quote state
				 */
				if (escaped == 0)
				{
					if (inquotes == true)
					{
						if (quotechar == c)
							inquotes = 0;
					}
					else
					{
						inquotes = 1;
						quotechar = c;
					}
				}
				return c;
				
			case '#':
				/**
				 * Skip comments initiated with a #
				 */
				if (inquotes == true)
					return c;
				else
				{
					singlelinecomment(configfile);
					break;
				}
				
			case '/':
				/**
				 * Skip comments initiated with // or /*
				 */
				if (inquotes == true)
				{
					return c;
				}
				else
				{
					c2 = getc (configfile);
					if (c2 == '/')
						singlelinecomment(configfile);
					else if (c2 == '*')
						multilinecomment(configfile);
					else
					{
						ungetc(c2,configfile);
						return c;
					}
					break;
				}
				
			default:
				/**
				 * Plain return any char we don't want to bother about
				 */
				return c;
		}
	}
}

static char config_unget_char (char c, FILE *configfile)
{
	if ((c == '"') || (c == '\''))
	{
		if (inquotes == true)
		{
			if (quotechar == c)
			{
				inquotes = false;
				quotechar = ' ';
			}
		}
		else
		{
			inquotes = true;
			quotechar = c;
		}
	}
	
	if ((c == '\\')&&(escaped == 2))
		escaped = 0;
	
	if (escaped == 1)
		escaped = 2;
	
	if ((c == '\n')||(c == '\r'))
		cis_config_line--;

	ungetc(c, configfile);
}

/**
 * Read though til the end of the line.
 */

static int singlelinecomment(FILE *configfile)
{
	/* This function reads to the end of the line then returns the bytes read. */
	int nbytes = 0;
	char c = ' ';

	while ((c = getc(configfile)) != EOF)
	{
		nbytes++;
		if (c == '\n')
		{
			ungetc(c, configfile);
			return --nbytes;
		}
	}
	
	printf("Warning: Unexpected EOF.  No newline at end of file!\n");
	
	return nbytes;
}

/**
 * This function reads to the next *\/ then returns the bytes read.
 */
static int multilinecomment(FILE *configfile)
{
	int nbytes = 0;
	char c = ' ';
	int splat = 0;

	while ((c = getc(configfile)) != EOF)
	{
		nbytes++;
	
		if ((c == '/') && (splat == 1))
		{
			return nbytes;
		}
		else if (c == '*')
			splat = 1;
		else
		{
			splat = 0;
			if (c == '\n')
				cis_config_line++;
		}
	}

	printf("Warning: Unexpected EOF. Unmatched /* !\n");
	
	return nbytes;
}

/**
 * Open a new file and read it in.
 */

static void cis_config_include(FILE *configfile, cis_config_node *context)
{
	char *oldfile;
	int oldline;
	char quote = ' ';
	char key[200];
	char *c = key;
	int i = 0;
	bool skip_whitespace = 0;
	FILE *newfile = NULL;
	
	/* Read file name */
	memset(key,0,200);
	while (1)
	{
		*c = config_get_char(configfile, skip_whitespace,1);
		skip_whitespace = 0;
		switch (*c)
		{
			case '\'':
			case '"':
				/* So it's a quoted string ... note the quote and move on... */
				if (c == key) /* first char */
					quote = *c;
				else if (*c == quote) /* matching */
				{
					if (*(c - 1) == '\\') /* escaped match */
						*(c - 1) = quote;
					else /* end of string */
						*(c++) = '\0';
				}
				else /* must be a literal, just keep going */
					c++;
				break;
							
				/* Ignore multiple whitespace and leading whitespace */
			case '\n':
			case '\r':
				skip_whitespace = 1;
			case '\t':
			case ' ':
				if (*(c - 1) == ' ')
					break;
				*(c++) = ' ';
				break;
							
			case '}':
				printf("Unexpected }\n");
				exit(1);
							
			case ';':
				if (quote == ' ')
					*(c++) = '\0';
				else 
					*(c++) = ';';
				break;
							
				/* Everything else: print it! */
			default:
				c++;
		}
		if (++i == 200)
		{
			printf("Out of value buffer\n");
			exit(1);
		}
		if ((key != c) && (*(c - 1) == '\0'))
			break;
	}
	newfile = fopen(key, "r");
	
	if (newfile == NULL)
	{
		fprintf(stderr,"Unable to open %s for reading.\n",key);
		exit(1);
	}
	
	oldfile = cis_config_file;
	oldline = cis_config_line;
	
	cis_config_file = strdup(key);
	cis_config_line = 0;
	
	cis_process_block(newfile,context,true); // Last param controls handling of EOF
	
	fclose(newfile);
	free(cis_config_file);
	
	cis_config_file = oldfile;
	cis_config_line = oldline;
}

/**
 * Load a module
 */

static void cis_config_load(FILE *configfile, cis_config_node *context)
{
	char quote = ' ';
	char key[200];
	char *c = key;
	int i = 0;
	bool skip_whitespace = 0;
	
	/* Read file name */
	memset(key,0,200);
	while (1)
	{
		*c = config_get_char(configfile, skip_whitespace,1);
		skip_whitespace = 0;
		switch (*c)
		{
			case '\'':
			case '"':
				/* So it's a quoted string ... note the quote and move on... */
				if (c == key) /* first char */
					quote = *c;
				else if (*c == quote) /* matching */
				{
					if (*(c - 1) == '\\') /* escaped match */
						*(c - 1) = quote;
					else /* end of string */
						*(c++) = '\0';
				}
				else /* must be a literal, just keep going */
					c++;
				break;
							
				/* Ignore multiple whitespace and leading whitespace */
			case '\n':
			case '\r':
				skip_whitespace = 1;
			case '\t':
			case ' ':
				if (*(c - 1) == ' ')
					break;
				*(c++) = ' ';
				break;
							
			case '}':
				printf("Unexpected }\n");
				exit(1);
							
			case ';':
				if (quote == ' ')
					*(c++) = '\0';
				else 
					*(c++) = ';';
				break;
							
				/* Everything else: print it! */
			default:
				c++;
		}
		if (++i == 200)
		{
			printf("Out of value buffer\n");
			exit(1);
		}
		if ((key != c) && (*(c - 1) == '\0'))
			break;
	}
	
	if (cis_module_load(key) == NULL)
	{
		printf("Loader error: attempted to load %s but apparently failed.\nTerminating.\n", key);
		exit(1);
	}

	printf("Loaded module: %s\n", key);
}

/**
 * Convert a block in to nodes on the tree
 */

static void cis_process_block (FILE *configfile, cis_config_node *context, bool included)
{
	char in;
	char quote = ' ';
	char *c;
	char key[200];
	int i = 0;
	bool skip_whitespace = 1;
	cis_config_node *child = NULL;
	
	while (1)
	{
		memset(key,0,200);
		i = 0;
		c = key;
		skip_whitespace = 1;
		/* Read key */
		while (1)
		{
			*c = config_get_char(configfile, skip_whitespace,1);
			skip_whitespace = 0;
			switch (*c)
			{
				/* Ignore multiple whitespace and leading whitespace */
				case '\n':
				case '\r':
					skip_whitespace = 1;
				case '\t':
				case ' ':
					if (*(c - 1) == ' ')
						break;
					*(c++) = ' ';
					break;
					
				case '}':
					printf("Unexpected }\n");
					exit(1);
					
				case ';':
					printf("Unexpected ;\n");
					exit(1);
					
				case '{':
					config_unget_char(*c,configfile);
				case ':':
				case '=':
					*(c++) = '\0';
					break;
					
				/* Everything else: print it! */
				default:
					c++;
			}
			if (++i == 200)
			{
				printf("Out of keybuffer\n");
				exit(1);
			}
			if ((key != c) && (*(c - 1) == '\0'))
				break;
			
			if ((i == 8) && (strncasecmp(key,"include ",8) == 0))
				break;
			if ((i == 5) && (strncasecmp(key,"load ",5) == 0))
				break;
		}
		c -= 2;
		while (*c == ' ')
		{
			*(c--) = '\0';
		}
		/* Skip past := combinations */
		while (1)
		{
			in = config_get_char(configfile, true, 1);
			if ((in != ':')&&(in != '='))
			{
				/* Okie, we're not allowing valueless keys to be used as flags anymore. */
				if (in == ';')
				{
					printf("Missing a value\n");
					exit(1);
				}
				
				/* Don't bother ungetting if it's a block opener */
				if (in != '{')
					config_unget_char(in,configfile);
				
				break;
			}
		}
		/* Process include specially */
		if (strncasecmp(key,"include",7) == 0)
		{
			/* Read the string and include it */
			cis_config_include(configfile,context);
		}
		else if (strncasecmp(key,"load",4) == 0)
		{
			/* Load a module ... */
			cis_config_load(configfile,context);
		}
		else
		{
			/* Allocate new node then process it as appropriate */
			child = smalloc(sizeof(cis_config_node));
			child->name = strdup(key);
			child->parent = context;
			linklist_add(context->children,child);
			if (in == '{')
			{
				/* Block */
				child->block = true;
				child->children = linklist_create();
				cis_process_block(configfile,child,false);
			}
			else
			{
				/* A string ... look, variable reuse :P */
				memset(key,0,200);
				i = 0;
				c = key;
				skip_whitespace = 0;
				quote = ' ';
				while (1)
				{
					*c = config_get_char(configfile, skip_whitespace,1);
					skip_whitespace = 0;
					switch (*c)
					{
						case '\'':
						case '"':
							/* So it's a quoted string ... note the quote and move on... */
							if (c == key) /* first char */
								quote = *c;
							else if (*c == quote) /* matching */
							{
								if (*(c - 1) == '\\') /* escaped match */
									*(c - 1) = quote;
								else /* end of string */
									*(c++) = '\0';
							}
							else /* must be a literal, just keep going */
								c++;
							break;
							
						/* Ignore multiple whitespace and leading whitespace */
						case '\n':
						case '\r':
							skip_whitespace = 1;
						case '\t':
						case ' ':
							if (*(c - 1) == ' ')
								break;
							*(c++) = ' ';
							break;
							
						case '}':
							printf("Unexpected }\n");
							exit(1);
							
						case ';':
							if (quote == ' ')
								*(c++) = '\0';
							else 
								*(c++) = ';';
							break;
							
							/* Everything else: print it! */
						default:
							c++;
					}
					if (++i == 200)
					{
						printf("Out of value buffer\n");
						exit(1);
					}
					if ((key != c) && (*(c - 1) == '\0'))
						break;
				}
				child->text = strdup(key);
			}
		}
		/* Chase any stray characters... */
		while (1)
		{
			in = config_get_char(configfile, true, 0);
			switch (in) {
				case EOF:
					if (included)
						return;
					printf("Unexpected EOF\n");
					exit(1);
					
				case '}':
					return;
					
				case ';':
					break;
					
				default:
					config_unget_char(in,configfile);
					in = 0;
					break;
			}
			if (in == 0)
				break;
		}
	} /* End of reading entities loop */
}

/**
 * Debug code... 
 */

static void print_dots (int i)
{
	int j = 0;
	while (i != j++)
		printf(".");
}
static void print_tree (cis_config_node *tree)
{
	static int depth = 0;
	linklist_iter *iter = NULL;
	cis_config_node *child = NULL;
	
	print_dots(depth);
	printf("%s", tree->name);
	
	if (tree->block)
	{
		printf("\n");
		
		print_dots(depth);
		printf("{\n");
		
		depth++;
		iter = linklist_iter_create(tree->children);
		while (child = linklist_iter_next(iter))
			print_tree(child);
		linklist_iter_free(iter);
		depth--;
		
		print_dots(depth);
		printf("}\n");
	}
	else
		printf(" := %s\n", tree->text);
	return;
}

/**************/

cis_handler_node *cis_config_handler_root = NULL;

/**
 * Add a handler to the tree
 */
cis_handler_node *cis_config_add_handler (cis_handler_node *location, unsigned char *name, int (*handler)(cis_config_node *node), void (*delete_callback)(cis_handler_node *location))
{
	cis_handler_node *temp = NULL;
	
	if (cis_config_handler_root == NULL)
	{
		cis_config_handler_root = smalloc(sizeof(cis_handler_node));
		cis_config_handler_root->children = map_create_init_from_preset(MAP_ALLOW_AZ09_SPACE_CI);
	}

	if (location == NULL)
		location = cis_config_handler_root;
	
	if (name == NULL)
	{
		printf("\nFatal Error: cis_config_add_handler called with empty name\nTerminating\n");
		exit(1);
	}
	
	temp = smalloc(sizeof(cis_handler_node));
	temp->name = strdup(name);
	temp->parent = location;
	temp->handler = handler;
	if (delete_callback)
		temp->delete_callback = delete_callback;

	temp->children = map_create_init_from_preset(MAP_ALLOW_AZ09_SPACE_CI);
	map_add(location->children, name, temp);
	return temp;
}

/**
 * Delete a handler and all it's children from the tree
 */

static void cis_config_del_child_handler(cis_handler_node *location)
{
	map_iter *iter = NULL;
	cis_handler_node *temp = NULL;
	
	if ((cis_config_handler_root == NULL)||(location == NULL))
		return;
	
	if (location->delete_callback)
		location->delete_callback(location);
	
	if (location->children->root.children > 0)
	{
		iter = map_iter_create(location->children);
		while (temp = map_iter_next(iter))
		{
			cis_config_del_child_handler(temp);
			map_iter_del(iter);
			free(temp);
		}
		map_iter_free(iter);
	}
	
	map_free(location->children);
	free(location->name);
	return;
}

void cis_config_del_handler(cis_handler_node *location)
{
	map_del(location->parent->children, location->name);
	cis_config_del_child_handler(location);
	free(location);
	return;
}

/**
 * Attempt to interpret tree
 */
 
static void cis_config_interpret_tree (cis_config_node *tree, cis_handler_node *context)
{
	linklist_iter *iter = NULL;
	cis_config_node *child = NULL;
	cis_handler_node *handler = NULL;
	int descent;
	
	if ((tree == NULL)||(context == NULL))
		return;
	
	if (tree->block)
	{
		iter = linklist_iter_create(tree->children);
		while (child = linklist_iter_next(iter))
		{
			descent = 0;
			handler = NULL;
			handler = map_find(context->children,child->name);
			if (handler)
			{
				if (handler->handler)
					descent = handler->handler(child);
					
				if (descent == 0)
					cis_config_interpret_tree(child,handler);
			}
		}
		linklist_iter_free(iter);
	}
	else
	{
		handler = map_find(context->children,tree->name);
		if ((handler)&&(handler->handler))
			handler->handler(tree);
	}
		
	return;
}

/**************/

/**
 * Nuke that tree baby!
 */
 
static void cis_config_nuke_tree (cis_config_node *tree)
{
	linklist_iter *iter = NULL;
	cis_config_node *child = NULL;
	
	if (tree == NULL)
		return;
	
	if (tree->block)
	{
		iter = linklist_iter_create(tree->children);
		while (child = linklist_iter_next(iter))
		{
			cis_config_nuke_tree(child);
			linklist_iter_del(iter);
		}
		linklist_iter_free(iter);
	}
	
	free(tree->name);
	if (tree->text)
		free(tree->text);
	if (tree->children)
		linklist_free(tree->children);
	free(tree);
}

/**************/

/**
 * Public function used to load and interpret a config file.
 */

int cis_load_config(unsigned char *filename)
{
	FILE *configfile = NULL;
	cis_config_node *tree = NULL;
	linklist_iter *iter;
	char *line;
	
	configfile = fopen(filename, "r");
	
	if (configfile == NULL)
	{
		fprintf(stderr,"Unable to open %s for reading.\n",filename);
		return 0;
	}
	
	tree = smalloc(sizeof(cis_config_node));
	tree->name = strdup(filename);
	tree->block = true;
	tree->children = linklist_create();
	
	cis_config_file = strdup(filename);
	cis_config_line = 0; 
	
	cis_process_block(configfile,tree,true); // Last param controls handling of EOF
	
	print_tree(tree);
	
	cis_config_interpret_tree(tree,cis_config_handler_root);
	
	cis_config_nuke_tree(tree);
	fclose(configfile);	
	free(cis_config_file);
	
	return 1;
}

#if 0

/**
 * Should it be needed...
 */

static int read_text_block (FILE *configfile, linklist_root *list)
{
	unsigned char buf[1000];
	unsigned char *c = buf;
	char *val;
	char in;
	bool readinline = 1;
	bool skip_whitespace = 1;
		
	/**
 	 * Just read in \n delimited lines ... return on }
 	 * We're really easy to please.
	 */
	while (1)
	{
		memset(buf, 0, 1000);
		c = buf;
		readinline = 1;
		
		while (readinline)
		{
			in = config_get_char(configfile, skip_whitespace);
			skip_whitespace = 0;
			
			switch (in)
			{
				case EOF:
					printf("Unexpected EOF\n");
					exit(1);
					
					/* Ignore multiple whitespace and leading whitespace */
				case '\t':
				case ' ':
					if ((*c == ' ')||(c == buf))
						break;
					*(c++) = ' ';
					break;
					
					/* \n and \r are end of line ... } is end of line and return */
				case '\n':
				case '\r':
					skip_whitespace = 1;
				case '}':
					*(c++) = '\0';
					if (*buf != '\0')
						linklist_add(list,strdup(buf));
					readinline = 0;
					if (in == '}')
						return;
					break;
					
					/* Escaping ... \ before end of line ignores the \n, all other combinations print the literals */
				case '\\':
					in = config_get_char(configfile,false);
					if (in == EOF)
					{
						printf("Unexpected EOF\n");
						exit(1);
					}
					if ((in == '\n')||(in == '\r'))
					{
						while ((in == '\n')||(in == '\r'))
							in = config_get_char(configfile,false);
						ungetc(in,configfile);
						break;
					}
					*(c++) = in;
					break;
					
				/* Everything else: print it! */
				default:
					*(c++) = in;
			}
		}
	}
}
#endif
