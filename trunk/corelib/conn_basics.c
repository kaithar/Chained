/**
 * @file main.c
 * @brief Basic abstracted connection methods.
 * Chained in Sanity.
 * Core Library.
 * This file contains methods that are designed to handle common basic tasks relating to connections.
 * These method's should work happily on any connection type.
 */

#include <chained.h>
#include <stdarg.h>		/* va_*() */

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
void conn_read_to_recvq (connection *cn)
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
		if (cn->recvq_size > 10000)
		{
			/* Um, that's a lot of data ... lets just let that empty a little .... */
			break;
		}

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
						cn->recvq_size += ((next - line) - 1);
						line = strdup(line);
						linklist_add(cn->recvq,line);
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

bool conn_send_from_sendq (connection *cn)
{
	char *line;
	linklist_iter *pos;
	/**
	 * If there is no sendq, return true so it'll remove it from the global_sendq
	 */
	
	if (cn->sendq->members == 0)
	{
		socketengine->mod(cn,0,-1);
		return 1;
	}
	
	pos = linklist_iter_create( cn->sendq );
	
	/**
	 * If the connection is closed, attempt to empty the sendq.
	 * Dead connections can recv messages though...
	 */
	
	if (cn->state.closed == 1)
	{
		while ((line = linklist_iter_next( pos )) != NULL)
		{
			linklist_iter_del(pos);
			free(line);
		}
		cn->sendq_size = 0;
		socketengine->mod(cn,0,-1);
		return 1;
	}	
	
	while ((line = linklist_iter_next( pos )) != NULL)
	{
		if (cn->write(cn,line) == 0)
			return 0;
		linklist_iter_del(pos);
		cn->sendq_size -= strlen(line);
		if (cn->sendq_size < 0)
			fprintf(stderr,"Um, negative sendq size?\n");
		free(line);
	}
	socketengine->mod(cn,0,-1);
	return 1;
}

/* Yeah, this is a plain and simple wrapper.
 * At some point I'll trial this against a custom printf variation, but for now it'll do.
 * (cause stuff like channel support is more important)
 */

static char sharbuf[10000];

int cprintf(connection *stream, char *fmt, ...)
{
	va_list argv;
	int nbytes = 0;
	char *temp;
	
	va_start(argv, fmt);
	nbytes = vsnprintf(sharbuf, 9999, fmt, argv);
	va_end(argv);
	
	/* Try and send it... */
	if (1||(stream->sendq->members != 0) || (stream->write(stream,sharbuf) == 0))
	{
		/* Well that didn't work ... better sendq it... */
		temp = strdup(sharbuf);
		linklist_add(stream->sendq,temp);
		stream->sendq_size += strlen(temp);
		if (stream->sendq->members == 1)
		{
			/* We just added the first line to the sendq ... lets listen for the ability to write */
			socketengine->mod(stream,0,1);
		}
	}
	
	return nbytes;
}
