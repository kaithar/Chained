#include <includes/chained.h>

#include <sys/socket.h>
#include <sys/types.h>

int cis_se_process ( )
{
	static connection *read_events[__MAXFDS__];
	static connection *write_events[__MAXFDS__];

	/** Grab some socket events to play with */
	int eventcount = r_config.socketengine->wait(read_events, write_events, r_config.patience);
	int r = 0, w = 0, i = 0;

	int temp_int, temp_int2;

	/** Run through the existing connections looking for data to read */
	for ( i = 0; (r + w) < eventcount; i++ )
	{
		if ( read_events[i] != NULL )
		{
			r++;
			if (read_events[i]->recvq != NULL)
				temp_int = read_events[i]->recvq->queue_size;
			read_events[i]->read(read_events[i]);

			if (
					(read_events[i]->recvq != NULL) &&
					(read_events[i]->recvq->queue_size > temp_int) &&
					(temp_int == 0)
				 )
			{
				fifo_add(r_config.global_recvq, read_events[i]);
			}
			/* Don't wait for data! too much to do! */
			r_config.patience = 10;
		}

		if ( write_events[i] != NULL )
		{
			w++;
			if ( write_events[i]->state.connecting == 1 )
			{
                                temp_int = temp_int2 = 0;
				if (getsockopt(write_events[i]->fd, SOL_SOCKET, SO_ERROR, &temp_int, &temp_int2) < 0)
                                {
                                    perror("getsockopt in se_process: ");
                                    abort();
                                }

				if ( temp_int == 0 )
				{
					write_events[i]->connected(write_events[i]);
					write_events[i]->state.connecting = 0;
				}
				else
				{
					write_events[i]->connect_failed(write_events[i], temp_int);
					write_events[i]->state.remote_closed = 1;
					write_events[i]->state.local_read_shutdown = 1;
					write_events[i]->state.local_write_shutdown = 1;
					r_config.socketengine->del(write_events[i]);
					cis_reap_connection(write_events[i]);
				}
			}
			else
				write_events[i]->write(write_events[i]);
		}
	} // foreach (event)

}