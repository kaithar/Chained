// Chained In Sanity
// select socket engineeeee

#include <chained.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/select.h>

static int se_s_add(connection *cn, int addread, int addwrite);
static int se_s_del(connection *cn);
static int se_s_wait(connection *read_events[], connection *write_events[], int howlong);

static fd_set rfds, wfds;
static fd_set temp_rfds, temp_wfds;

void cis_load_selectengine (void)
{
	// Select module init
	socketengine = smalloc(sizeof(socket_engine));
	socketengine->add = &se_s_add;
	socketengine->del = &se_s_del;
	socketengine->wait = &se_s_wait;
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	return;
}

static int se_s_add(connection *cn, int addread, int addwrite) 
{
	if (addread) FD_SET(cn->fd, &rfds);
	if (addwrite) FD_SET(cn->fd, &wfds);
	socketengine->cns[cn->fd] = cn;
	socketengine->fdcount++;
	return 0;
}

static int se_s_del(connection *cn)
{
	FD_CLR(cn->fd, &rfds);
	FD_CLR(cn->fd, &wfds);
	socketengine->cns[cn->fd] = NULL;
	socketengine->fdcount--;
	return 0;
}

static int se_s_wait(connection *read_events[], connection *write_events[], int howlong)
{
	int i,w,r;
	int eventcount = 0;
	struct timeval tv;
	
	if (howlong >= 1000)
		fprintf(stderr,"Requested wait of %d ms is too long! Waiting for %d instead.\n",howlong,howlong%1000);
	
	tv.tv_sec = 0;
	tv.tv_usec = howlong % 1000; // Time out at 1/4 second for now ... this will be adjusted later

	temp_rfds = rfds; // This seems to be perfectly safe, 
	temp_wfds = wfds; // please let me know if it can be demonstrated not be

	eventcount = select(__MAXFDS__, &temp_rfds, &temp_wfds, NULL, &tv);

	w = r = 0;
	
	if (eventcount > 0)
	{
		// Okie, this is likely to be insanly inefficent ... anyone got a better idea? Answers to #coding please :)
		for (i = 0; i < __MAXFDS__; i++)
		{
			if (FD_ISSET(i,&temp_rfds))
				read_events[r++] = socketengine->cns[i];
			if (FD_ISSET(i,&temp_wfds))
				write_events[w++] = socketengine->cns[i];
			if (r+w == eventcount)
				break;
		}
	}
	
	read_events[r] = NULL;
	write_events[w] = NULL;
	
	return eventcount;
}
