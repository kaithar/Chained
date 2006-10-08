/**
 * @file strings.c
 * This file contains various functions for simplifing the handling of strings.
 */

/**
 * I break a line according to the provided end of line characters.
 * 
 * The return value is either:
 * - NULL (When no matching eol character is found)
 * - A pointer to the character after the first match (The eol itself will be replaced with a NULL)
 */
 
unsigned char *lineBreaker(unsigned char *line, char *eol, int eols)
{
	unsigned char *cptr = line;
	int i = 0;
	
	while (1)
	{
		if (*cptr == '\0')
			return NULL;
	
		for (i = 0; i < eols; i++)
		{
			if (*cptr == eol[i])
			{
				*(cptr++) = '\0';
				return cptr;
			}
		}

		cptr++;
	}
}
