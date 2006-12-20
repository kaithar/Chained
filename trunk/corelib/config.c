/**
 * @file config.c
 * @brief Config reader.
 * Chained in Sanity.
 * Core Library.
 * This file reads in a config.
 */

#include "chained.h"
#include <string.h>

int inc_line_num () {}
int getLineNum() {}

static int singlelinecomment(FILE *configfile);
static int multilinecomment(FILE *configfile);

static void cis_config_include(FILE *configfile, cis_config_node *context);
static void cis_process_block (FILE *configfile, cis_config_node *context, bool included);


static char config_get_char (FILE *configfile, bool skip_whitespace, bool fatal_eof)
{
	static bool inquotes = false;
	static char quotechar = '\0';
	char c,c2;
	static int escaped = 0;
	
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

static int multilinecomment(FILE *configfile)
{
	/* This function reads to the next *\/ then returns the bytes read. */
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
				inc_line_num();
		}
	}

	printf("Warning: Unexpected EOF. Unmatched /* !\n");
	
	return nbytes;
}

static void cis_config_include(FILE *configfile, cis_config_node *context)
{
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
	
	cis_process_block(newfile,context,true); // Last param controls handling of EOF
}

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
					ungetc(*c,configfile);
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
			
			if ((i == 7) && (strncasecmp(key,"include",8) == 0))
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
					ungetc(in,configfile);
				
				break;
			}
		}
		/* Process include specially */
		if (strncasecmp(key,"include",8) == 0)
		{
			/* Read the string and include it */
			cis_config_include(configfile,context);
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
					ungetc(in,configfile);
					in = 0;
					break;
			}
			if (in == 0)
				break;
		}
	} /* End of reading entities loop */
}

/* Debug code... */

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
	
	cis_process_block(configfile,tree,true); // Last param controls handling of EOF
	
	print_tree(tree);
	
	exit(0);
}
