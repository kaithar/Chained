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

static char config_get_char (FILE *configfile, bool skip_whitespace)
{
	static bool inquotes = false;
	static char quotechar = '\0';
	static int escaped = 0;
	char c,c2;
	
	while (1)
	{
		c = getc(configfile);
		
		/* \ sets escaped to 2, the next char has escaped 1, the one after escaped 0 */
		if (--escaped < 0)
			escaped = 0;
		
		switch (c)
		{
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

#if 0
int read_keyed_block (FILE *configfile, map_root *map, bool included)
{
	int (*keyword_handler)() = NULL;
	int breakout = 0;
	int nbytes = 0;
	char in = ' ';
	unsigned char buf[1000];
	unsigned char *c = buf;
	unsigned char *value = NULL;
	char *dupvalue = NULL;
	int matchchar = ' ';	/* by default we're looking for a word. */
	//config_context *nested = NULL;

	/* Decrease when we return; */
	Cdepth++;
	iCdepth = Cdepth;	/* This is the inital value ... the idea being that when iCdepth == Cdepth we're at the same point we started at. */

	while (1)
	{
		memset(buf, 0, 1000);
		nbytes = 0;
		c = buf;
		value = NULL;
		dupvalue = NULL;

		/*
		* Welcome to the begining of the line!
		* First strip white space (Note: ; is counted as white space so empty statements are ignored.)
		* Invalid characters and match char set here for efficency
		*/
		while (1)
		{
			in = config_get_char(configfile,true);

			switch (in)
			{
				case EOF:
					/* Return; */
					if (included == 1)
						return 1;
					else
					{
						fprintf(stderr,"Unexpected EOF\n");
						exit(1);
					}
					
					/* Open and close blocks */
//				case '{': case '(':
					/* Opening block, recurse! */
//					readBlock(configfile, context, 0);
//					break;
				case '}': case ')':
					/* Closing character, callback and return! */
					if (included == 0)
						return 1;
					else
					{
						printf("Unexpected }\n");
						exit(1);
					}
					
					/* Characters that should not be here! */
				case '<':
					/* Variable, disallowed */
				case '[':
					/* Subscript, disallowed */
				case ']': case '>':
					/* Closers, disallowed */
				case '=': case ':':
					/* Assignment, disallowed */
					printf("Unexpected %c, Expected keyword\n", in);
					exit(1);
					
				/* Considered to be white space */
				case ';':
					break;
					
					/* These cases are correct and indicate start of keywords! */
				case '\'': case '"':
					/*
					 * This is a quoted keyword ... weird, but allowed
					 */
					matchchar = in;
					in = config_get_char(configfile,false);
					breakout = 1;
					break;
					
				default:
					matchchar = ' ';
					breakout = 1;
					break;
			}
			if (breakout)
				break;
		}
		breakout = 0;
		
		/*
		* should be the begining of the keyword
		* if it's a block start key, recurse with no change of context ...
		* otherwise read the string in.
		*/
		
		/* First unget so the first get is the first char of the keyword ... */
		ungetc(in, configfile);
		/*
		* Now read!
		* We do it this way so we can place the reading and length tests at the begining and thus avoid any unpleasent bugs
		*/
		while (1)
		{
			/* This point is reached at the begining of each word (for non quoted keywords) */
			while (1)
			{
				if (nbytes >= 999)
				{
					printf("Keyword too big!\n");
					exit(1);
				}
	
				in = getc(configfile);
				
				/* Loop over characters in word */
				breakout = 0;
				switch (in)
				{
					case EOF:
						printf("Unexpected EOF\n");
						exit(1);
					case '#':
						/* Comment, read rest of line */
						if (matchchar == ' ')
							singlelinecomment(configfile);
						else
						{
							*(c++) = in;
							nbytes++;
						}
				
						break;
					case '/':
						/* Possible comment ... allow / in keywords for now. */
						if (matchchar == ' ')
						{
							in = getc(configfile);
							if (in == '/')
								singlelinecomment(configfile);
							else if (in == '*')
								multilinecomment(configfile);
							else
							{
								*(c++) = '/';
								ungetc(in, configfile);
								nbytes++;
							}
						}
						else
						{
							*(c++) = in;
							nbytes++;
						}
						
						break;
					case '(': case '{': case '[': case '<':
					case ')': case '}': case ']': case '>':
						printf("Character %c is not allowed in keywords!\n", in);
						exit(1);
					case ';': case ':': case '=':
						ungetc(in, configfile);
						if (matchchar != ' ')
						{
							printf("Character %c is not allowed in keywords!\n", in);
							exit(1);
						}
				
						/* Ordering of \n \r \t default is important! */
					case '\n': case '\r':
						if ((in == '\n') || (in == '\r'))
							inc_line_num();
					case '\t':
						if (matchchar == ' ')
							in = ' ';
					default:
						if (in == matchchar)
						{
							breakout = 1;
							break;
						}
						
						*(c++) = in;
						nbytes++;
				}
				if (breakout == 1)
					break;
			}
			breakout = 0;
			*c = '\0';

			if (map_find(context->keywords, buf) != NULL)
			{
				/* This is a valid keyword */
				c++;
				nbytes++;
				break;
			}
			else
			{
				/* This is not a valid keyword. */
				in = getc(configfile);
			
				if ((matchchar == ' ') && (in != ';') && (in != ':') && (in != '='))
				{
					*(c++) = ' ';
					nbytes++;
					ungetc(in, configfile);
					continue;
				}
				else
				{
					printf("Unknown keyword '%s'\n", buf);
					exit(1);
				}
			}
			/* End of "word" loop */
		}

		/*
		* At this point we have a valid keyword in buffer, now we need to find the value
		* More whitespace strip!
		*/
		while (1)
		{
			in = getc(configfile);

			switch (in)
			{
				case EOF:
					printf("Unexpected EOF\n");
					exit(1);
				case '\n': case '\r':
					inc_line_num();
				case '\t': case ' ': case ':': case '=':
					break;
				default:
					breakout = 1;
			}
			if (breakout)
				break;
		}
		breakout = 0;

		if (in == ';')
		{
			/* End of command, but no value.  Store "1" as the value. */
			dupvalue = strdup("1");
			map_add(context->values, buf, dupvalue);
			keyword_handler = map_find(context->keyword_callbacks, buf);

			if (keyword_handler != NULL)
				keyword_handler(context, dupvalue);
			
			continue;
		}

		if ((in == '{') || (in == '('))
		{
			/* Value is a nested block. */
			nested = map_find(context->child_context, buf);

			if (nested == NULL)
			{
				printf("Keyword '%s' does not take a block as it's value\n", buf);
				exit(1);
			}
			
			readBlock(configfile, nested, 0);
			nested = NULL;

			/*
			* Don't worry too much about a ; ...
			* if it isn't there then oh well; if it is then the white space matcher at the begining of statement will catch it.
			*/
			continue;
		}
		else
		{
			/*
			* Either a quoted string or a single word.
			* ignore <> and [] for now, will be added later!
			*/
			if ((in == '\'') || (in == '"'))
			{
				matchchar = in;	/* Value is a quoted string! */
				in = getc(configfile);
			}
			else
				matchchar = ' ';
			
			value = c;
			/* Ok, read it in */
			while (1)
			{
				/* Loop over characters in word */
				breakout = 0;
				switch (in)
				{
					case EOF:
						printf("Unexpected EOF\n");
						exit(1);
					case ';':
						if ((matchchar == ' ') || (matchchar == ';'))
							ungetc(in, configfile);
						
						/* The order of \n \r \t default is important! */
					case '\n': case '\r':
						if ((in == '\n') || (in == '\r'))
							inc_line_num();
					case '\t':
						if ((matchchar == ' ') || (matchchar == ';'))
							in = matchchar;
					default:
						if (in == matchchar)
						{
							breakout = 1;
							break;
						}
						
						*(c++) = in;
						nbytes++;
				}
				if (breakout == 1)
					break;
				if (nbytes == 1000)
				{
					printf("Keyword and value too big!\n");
					exit(1);
				}
				in = getc(configfile);
			}
			
			*c = '\0';
			
			/* Value should now be complete! */
			dupvalue = strdup((char *)value);
			
			if (DEBUG)
				printf("%s:%d %s:%s\n", get_config_file(), get_line_num(), buf, value);
			
			map_add(context->values, buf, dupvalue);
			keyword_handler = map_find(context->keyword_callbacks, buf);
			
			if (keyword_handler != NULL)
				keyword_handler(context, dupvalue);
			/* End of value grabber! */
		}

		/*
		* Find the remaining ;
		* End of block will be handled by the inital whitespace trimmer code, so this just looks for ;
		*/
		while (1)
		{
			in = getc(configfile);
			switch (in)
			{
				case EOF:
					printf("Unexpected EOF\n");
					exit(1);
					
					/* Case ordering important! */
				case '\n': case '\r':
					inc_line_num();
				case '\t': case ' ':
					break;
				default:
					breakout = 1;
			}
			if (breakout)
				break;
		}
		breakout = 0;

		if (in != ';')
		{
			printf("Unterminated block\n");
			exit(1);
		}
		
		/* End of "line" loop */
	}

	/* End of reader */
	printf("huh? %s:%d\n", get_config_file(), get_line_num());
	context_free_instants(context);

	return 0;
}

#endif


int cis_load_config(unsigned char *filename)
{
	FILE *configfile = NULL;
	linklist_root *list = NULL;
	linklist_iter *iter;
	char *line;
	
	list = linklist_create();
	
	configfile = fopen(filename, "r");
	
	if (configfile == NULL)
	{
		fprintf(stderr,"Unable to open %s for reading.\n",filename);
		linklist_free(list);
		return 0;
	}
	
	read_text_block (configfile, list);
	
	iter = linklist_iter_create(list);
	
	while (line = linklist_iter_next(iter))
	{
		printf("%s\n",line);
	}
	exit(0);
}
