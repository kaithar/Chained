/**
 * @file main.c
 * @brief Basic abstracted connection methods.
 * Chained in Sanity.
 * Core Library.
 * This file contains methods that are designed to handle common basic tasks relating to connections.
 * These method's should work happily on any connection type.
 */

#include <chained.h>

/**
 * This buffer is used as a temp read in.
 * It's 10k big ... if you try and accept lines bigger than that, I will have to hurt you.
 */
char conn_read_in_temp[10000];

/**
 * I read new line delimited data from the connection and add it to the sendq.
 * This function relys on the connection allocating enough buffer space for the largest line it could expect to receive.
 * If the connection tries to send too bigger line, it gets hard wrapped.
 * It's rather important to note that this function prefers efficent memory use over efficent cpu usage.
 * This means that if we expect a max line of 5k, we use a 5k temp buffer and then malloc and memcpy.
 * This is in contrast to doing a malloc of 5k every time.
 *
 * We're allocating a block for each line...
 * I'm far too lazy to try and keep track of multiple messages in the same block of memory.
 * Honest.
 */
void conn_read_to_sendq (connection *cn)
{
	int readin = 0;
	
	int nbytes = 0;
	int tnbytes = 0;
	
	char *next = NULL;
	char *line = conn_read_in_temp;
	
	int foo = 0;
	
	readin = cn->recvq_buf_free + cn->recvq_buf_used;
		
	while (1)
	{
		memset(&conn_read_in_temp,0,readin);
		line = conn_read_in_temp;
		if ((nbytes = cn->read(cn, readin, conn_read_in_temp)) <= 0) 
		{
			/* We expect cn->read to handle it's own errors and return 0 when we need to stop reading. */
			break;
		}
		else
		{
			tnbytes += nbytes;
			
			/**
			 * Split on \n or \r ...
			 * 
			 * lineBreaker returns NULL on no match,
			 * or makes the newline char a NULL and returns a pointer to the char after the newline 
			 */
			while ((next = lineBreaker(line,"\n\r",2)) != NULL)
			{
				if (*cn->recvq_buf != 0) // If there is something in the buffer from the last read, bang this line on the end.
				{
					strncat(cn->recvq_buf, line, (cn->recvq_buf_free - 1) );
					line = strdup(cn->recvq_buf);
					
					/* Wipe the connection's recvq buffer */
					memset(cn->recvq_buf, 0, (cn->recvq_buf_free + cn->recvq_buf_used) );
					cn->recvq_buf_free = readin;
					cn->recvq_buf_used = 0;
					
					linklist_add(cn->recvq,line);
					cn->recvq_size += strlen(line);
				}
				else
				{
					if (*line != '\0') 
					{
						line = strdup(line);
						linklist_add(cn->recvq,line);
						cn->recvq_size += ((next - line) - 1);
					}
				}
				line = next;
			}
			strncat(cn->recvq_buf, line, (cn->recvq_buf_free - 1) );
			foo = strlen(line);
			cn->recvq_buf_free -= foo;
			cn->recvq_buf_used += foo;
			foo = 0;
		}
	}
}
