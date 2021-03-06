#include <libchained/chained.h>

int ssl_write_test(connection *cn, unsigned char *line)
{
	static int oneshot = 0;
	printf("%d> %s\n",cn->fd,line);
	if (oneshot == 0)
	{
		cprintf(cn,"Success!\n");
		oneshot = 1;
	}
}

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
	printf("New connection from %s!\n",cn->source);
	cn->callback_read = &raw_printer;
	cn->callback_close = &close_callback;
	cprintf(cn,"Hi there, %s!\n", cn->source);
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
	cis_modules_set_path("./");
	cis_init();
	cis_load_selectengine();
	
	handler_test = cis_config_add_handler(NULL,"test block",&testcallback,&testdelcallback);
	cis_config_add_handler(handler_test,"bleh",&testcallback,&testdelcallback);
	
	cis_load_config("test.conf");
		
	test = ipv4_tcp_listen("test","0.0.0.0",2335);
		
	if (test != NULL)
	{
		test->callback_accept = &accept_callback;
		test->callback_close = &close_callback;
	}

#ifdef __CIS_HAS_OPENSSL__

	test = ipv4_tcp_listen("test-ssl","0.0.0.0",2336);
	
	if (test != NULL)
	{
		test->callback_accept = &accept_callback;
		test->callback_close = &close_callback;
		cis_openssl_port_upgrade(test);
	}
	else
	{
		test = ipv4_tcp_connect("test-outgoing-ssl","127.0.0.1",2336);
		
		if (test != NULL)
		{
			test->callback_read = &ssl_write_test;
			test->callback_close = &close_callback;
			cis_openssl_client_upgrade(test);
			cprintf(test,"What are you waiting for?\n");
		}
	}

#endif

	printf("Going loopy...\n");
	cis_run();
}
