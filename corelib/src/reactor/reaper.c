#include <includes/chained.h>

/**
 * @brief Reaper...
 * This is where we store sockets we're waiting to reap.
 * Using a fifo here would have been nice, but we really do need to be able to iterate.
 * @internal
 */

static linklist_root *reaper_list = NULL;

void cis_init_reaper()
{
	reaper_list = linklist_create();
}

/**
 * @brief I add a connection to the kill list
 * If you force the code to reap a connection twice, it will most likely do a segfault.
 */

void cis_reap_connection(connection *cn)
{
	if (cn->state.reaping == 1)
		return;
	linklist_add(reaper_list,cn);
	cn->state.reaping = 1;
}

void cis_reaper_process ( )
{
  linklist_iter *reaper_iter = NULL;
  connection *temp = NULL;

	/** Attempt to reap connections... */
	reaper_iter = linklist_iter_create(reaper_list);
	while ( temp = linklist_iter_next(reaper_iter) )
	{
		/* Has this connection reached the end of it's life? */
		if ( ((temp->state.remote_closed == 1) && (temp->recvq->queue_size == 0)) ||
				 (temp->state.local_read_shutdown == 1) && (temp->sendq->queue_size == 0) )
		{
			/* Remove it from any relevant queues... */
			linklist_iter_del(reaper_iter);
			fifo_del(r_config.global_recvq, temp);
			/* Make sure it's all closed down... */
			temp->state.local_write_shutdown = 1;
			temp->close(temp);
			/* Free the important stuff.... */
			if ( temp->recvq ) buffer_free(temp->recvq);
			if ( temp->sendq ) buffer_free(temp->sendq);
			sfree(temp);
		}
	}
	linklist_iter_free(reaper_iter);
}
