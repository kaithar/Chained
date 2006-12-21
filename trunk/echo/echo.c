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

int testcallback (cis_config_node *test)
{
	printf("Callback for %s\n",test->name);
	return 0;
}

void testdelcallback (cis_handler_node *test)
{
	printf("Delete Callback for %s\n",test->name);
}

int main ()
{
	connection *test;
	cis_handler_node *handler_test;
	
	cis_drop_cores();
	cis_init();
	cis_load_selectengine();
	
	handler_test = cis_config_add_handler(NULL,"test block",&testcallback,&testdelcallback);
	cis_config_add_handler(handler_test,"bleh",&testcallback,&testdelcallback);
	
	cis_load_config("test.conf");
		
	test = ipv4_tcp_listen("test","192.168.0.70",2335);
	
	if (test != NULL)
	{
		test->callback_accept = &accept_callback;
		test->callback_close = &close_callback;
	}
	
	cis_run();
}
