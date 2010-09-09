#include <includes/chained.h>

void cis_readq_process ( )
{
  int i = 0;
  connection *temp = NULL;

	/** Process some of the readq */

	if ( r_config.global_recvq->members > 0 )
	{
		for ( i = 0; i <= 20; i++ )
		{
			temp = fifo_pop(r_config.global_recvq);
			if ( temp == NULL )
			{
				/* We seem to be out of connections to process... Have more patience waiting for new data ...*/
				r_config.patience = 250;
				break;
			}
			if (
							(temp->state.local_read_shutdown == 0) &&
							(temp->recvq->queue_size > 0)
					)
			{
				/* Local dead connections don't get processed ... remote dead ones -do- (since they died after sending this...) */
				if ( temp->callback_read )
					temp->callback_read(temp);
				else
					buffer_empty(temp->recvq);

				fifo_del(r_config.global_recvq, temp);
				
				/* If it has more messages left, do another pass later... */
				if ( temp->recvq->queue_size > 0 )
					fifo_add(r_config.global_recvq, temp);

				if ( r_config.global_recvq->members == 0 )
				{
					r_config.patience = 250; /* Nothing to do, so I don't mind wait a while */
					break;
				}
			}
			else
			{
				/* So this is either locally dead, or has no recvq ... both could happen, but ignore it either way */
				if (temp->recvq->queue_size > 0 )
				{
					/* Must be locally dead but with a recvq still ... this shouldn't be possible? */
					buffer_empty(temp->recvq);
				}

				/* In any case, if we're ignoring it, might as well remove it completely ... */
				fifo_del(r_config.global_recvq, temp);

				/* Since we didn't do much with this one, we'll try another pass... */
				i--;
			}
		}
	}
	else
		r_config.patience = 250;
}
