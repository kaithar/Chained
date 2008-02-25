/*
 * Chained in Sanity
 * IPv4 TCP abstractions
 * This module abstracts the stuff that is particular to tcp over ipv4
 */

#include <chained.h>

#include <fcntl.h>		/* O_NONBLOCK F_GETFL F_SETFL */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

connection *ipv4_tcp_listen(char *name, char *ip, int target_port);
static int ipv4_tcp_accept(connection *conn, int dummyi, char *dummyc);
connection *ipv4_tcp_connect (char *stream_name, char *target_host, int target_port);
int ipv4_tcp_read(connection *, int how_much, char *buffer);
int ipv4_tcp_write(connection *stream, char *str);
int ipv4_tcp_close(connection *stream);


connection *ipv4_tcp_listen(char *name, char *target_ip, int target_port)
{
	char *ip = "0.0.0.0";
	connection *conn;
	struct sockaddr_in our_addr;	/* Target for connection */
	int flags = 0;
	int opts = 0;

	if (target_ip != NULL)
		ip = target_ip;

	conn = smalloc(sizeof(connection));

	our_addr.sin_family = AF_INET;
	our_addr.sin_port = htons(target_port);	/* Short network byte order */
	our_addr.sin_addr.s_addr = inet_addr(ip);
	memset(&(our_addr.sin_zero), '\0', 8);	/* zero the rest */

	printf("Listen: IPv4: Binding to %s, port %d\n", ip, target_port);

	if ((conn->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("TCP socket");
		free(conn);
		return NULL;
	}

	opts = 1;

	if (setsockopt(conn->fd, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(opts)) < 0)
	{
		perror("Sockopts");
		close(conn->fd);
		free(conn);
		return NULL;
	}

	if (bind(conn->fd, (struct sockaddr *)&our_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("TCP bind");
		close(conn->fd);
		free(conn);
		return NULL;
	}

	if (listen(conn->fd, 10) == -1)
	{
		perror("TCP listen");
		close(conn->fd);
		free(conn);
		return NULL;
	}

	/* Make it nonblock and add it to the sockengine */
	flags = fcntl(conn->fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(conn->fd, F_SETFL, flags);

	/* Params are file descriptor, check for readable?, check for writable? */
	socketengine->add(conn, 1, 0);

	conn->name = strdup(name);
	conn->read = &ipv4_tcp_accept;
	conn->close = &ipv4_tcp_close;
	
	conn->recvq = fifo_create(); /** FIXME: Does a listening socket actually need a recvq/sendq? Why? */
	conn->sendq = fifo_create();

	printf("Bind complete, fd: %d!\n", conn->fd);

	return conn;
}

/**
 * Because this is a pseudo read abstraction, it must pretend to read data...
 */

static int ipv4_tcp_accept(connection *conn, int dummyi, char *dummyc)
{
	connection *newStream = NULL;
	int flags = 0;
	unsigned int addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in their_addr;

	newStream = smalloc(sizeof(connection));
	memset(&their_addr, 0, sizeof(struct sockaddr_in));

	if ((newStream->fd = accept(conn->fd, (struct sockaddr *)&their_addr, &addrlen)) < 0)
	{
		perror("TCP Accept");
		free(newStream);
		return 0;
	}
	
	newStream->name = strdup(conn->name);
	newStream->read = &ipv4_tcp_read;
	newStream->write = &ipv4_tcp_write;
	newStream->close = &ipv4_tcp_close;
  
	newStream->recvq = fifo_create();
	newStream->recvq_buf = smalloc(5000);
	newStream->recvq_buf_free = 5000;
  
	newStream->sendq = fifo_create();
	newStream->sendq_buf = smalloc(5000);
	newStream->sendq_buf_free = 5000;
	
	/* Make it nonblock and add it to the sockengine */
	flags = fcntl(newStream->fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(newStream->fd, F_SETFL, flags);
	
	socketengine->add(newStream, 1, 0);
	
	if (conn->callback_accept)
		conn->callback_accept(conn,newStream);
	
	return 0;
}

/**
 * @todo Make this properly async!
 */

connection *ipv4_tcp_connect (char *stream_name, char *target_host, int target_port) {
	struct hostent *dns_target = NULL;
	struct in_addr foo;
	struct sockaddr_in their_addr; // Target for connection
	connection *conn = NULL;
	int flags = 0;

	conn = smalloc(sizeof(connection));
  
  // DNS it ... this is currently liable to cause a lock up
	dns_target = gethostbyname(target_host);
  
	if (dns_target == NULL)
	{
		fprintf(stderr,"TCP Client: Failed to connect to %s:%d ... Unknown host\n",target_host,target_port);
		return NULL;
	}
  
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(target_port); // Short network byte order
	memcpy(&foo,*(dns_target->h_addr_list),sizeof(struct in_addr));
	memcpy(&their_addr.sin_addr.s_addr,*(dns_target->h_addr_list),sizeof(struct in_addr));
	memset(&(their_addr.sin_zero), '\0', 8); // zero the rest
  
	fprintf(stderr,"TCP Client: Connecting to %s (%s), port %d\n",target_host,inet_ntoa(foo),target_port);

	if ((conn->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("TCP socket");
		free(conn);
		return NULL;
	}  

	if (connect(conn->fd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
		perror("TCP connect");
		free(conn);
		return NULL;
	}

  // Make it nonblock and add it to the sockengine
	flags = fcntl(conn->fd,F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(conn->fd,F_SETFL,flags);
  // Params are file descriptor, check for readable?, check for writable?
	socketengine->add(conn,1,0);

	conn->read = &ipv4_tcp_read;
	conn->write = &ipv4_tcp_write;
	conn->close = &ipv4_tcp_close;
  
	conn->recvq = fifo_create();
	conn->recvq_buf = smalloc(5000);
	conn->recvq_buf_free = 5000;
  
	conn->sendq = fifo_create();
	conn->sendq_buf = smalloc(5000);
	conn->sendq_buf_free = 5000;
	
	fprintf(stderr,"Connect complete, fd: %d!\n",conn->fd);
	return conn;
}

int ipv4_tcp_read(connection *cn, int how_much, char *buffer)
{
	int nbytes = 0;
	
	if (cn->state.closed)
	{
		*buffer = '\0';
		return 0;
	}
	
	if ((nbytes = recv(cn->fd, buffer, how_much-1, 0)) <= 0)
	{
		if (nbytes == 0)
		{
			cn->close(cn);
			return 0;
		}
		else if (errno == EAGAIN)
		{
			return 0;
		}
		perror("recv");
		cn->close(cn);
		return 0;
	}
	buffer[nbytes] = '\0';
	return nbytes;
}

int ipv4_tcp_write(connection *stream, char *str)
{
	int nbytes = 0;
	
	if (stream->state.closed)
		return strlen(str);
	
	if ((nbytes = send(stream->fd, str, strlen(str), 0)) <= 0)
	{
		if (errno = EAGAIN)
		{
			return 0;
		}
		perror("send");
		stream->close(stream);
		return 0;
	}
	
	return nbytes;
}

int ipv4_tcp_close(connection *stream)
{
	if (stream->state.closed == 0)
	{
		if (stream->callback_close)
			stream->callback_close(stream);
		if (stream->enc_close)
			stream->enc_close(stream);
		stream->state.closed = 1;
		socketengine->del(stream);
		return close(stream->fd);
	}

	return 0;
}
