#include <chained.h>

int raw_printer(connection *cn, unsigned char *line)
{
	printf("%d> %s\n",cn->fd,line);
	cprintf(cn,"%s\n",line);
}

int close_callback(connection *cn)
{
	printf ("%d -- connection closed\n",cn->fd);
}

int accept_callback (connection *parent, connection *cn)
{
	printf("New connection!\n");
	cn->callback_read = &raw_printer;
	cn->callback_close = &close_callback;
	cprintf(cn,"Hi there!\n");
}

int main ()
{
	connection *test;
	
	cis_drop_cores();
	cis_init();
	cis_load_selectengine();
	
	test = ipv4_tcp_listen("test","192.168.0.70",2335);
	
	if (test != NULL)
	{
		test->callback_accept = &accept_callback;
		test->callback_close = &close_callback;
	}
	
	cis_run();
}
