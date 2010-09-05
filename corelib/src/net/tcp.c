/*
 * Chained in Sanity
 * IPv4 TCP abstractions
 * This module abstracts the stuff that is particular to tcp over ipv4
 */

#include "includes/chained.h"

#include <fcntl.h>    /* O_NONBLOCK F_GETFL F_SETFL */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>

connection *cis_tcp_listen(char *target_ip, int target_port);
static int cis_tcp_accept(connection *conn);
connection *cis_tcp_connect(char *target_host, int target_port);
int cis_tcp_read(connection *);
int cis_tcp_write(connection *stream);
int cis_tcp_close(connection *stream);


connection *cis_tcp_listen(char *target_ip, int target_port)
{
  char *ip = "0.0.0.0";
  connection *conn;

  struct sockaddr_in our_addr;  /* Target for connection */
  int flags = 0;
  int opts = 0;

  if (target_ip != NULL)
    ip = target_ip;

  conn = smalloc(sizeof(connection));

  our_addr.sin_family = AF_INET;
  our_addr.sin_port = htons(target_port);     /* Short network byte order */
  our_addr.sin_addr.s_addr = inet_addr(ip);
  memset(& (our_addr.sin_zero), '\0', 8);       /* zero the rest */

  printf("TCP: Listen: Binding to %s, port %d\n", ip, target_port);

  if ((conn->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("TCP socket");
    sfree(conn);
    return NULL;
  }

  opts = 1;

  if (setsockopt(conn->fd, SOL_SOCKET, SO_REUSEADDR, &opts, sizeof(opts)) < 0)
  {
    perror("Sockopts");
    close(conn->fd);
    sfree(conn);
    return NULL;
  }

  if (bind(conn->fd, (struct sockaddr *) &our_addr, sizeof(struct sockaddr)) == -1)
  {
    perror("TCP bind");
    close(conn->fd);
    sfree(conn);
    return NULL;
  }

  if (listen(conn->fd, 10) == -1)
  {
    perror("TCP listen");
    close(conn->fd);
    sfree(conn);
    return NULL;
  }

  /* Make it nonblock and add it to the sockengine */
  flags = fcntl(conn->fd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(conn->fd, F_SETFL, flags);

  /* Params are file descriptor, check for readable?, check for writable? */
  r_config.socketengine->add(conn, 1, 0);
  conn->read = &cis_tcp_accept;
  conn->close = &cis_tcp_close;

  printf("TCP: Listen: Bind complete, fd: %d!\n", conn->fd);

  return conn;
}

/**
 * Because this is a pseudo read abstraction, it must pretend to read data...
 */

static int cis_tcp_accept(connection *conn)
{
  connection *newStream = NULL;
  int flags = 0;
  unsigned int addrlen = sizeof(struct sockaddr_in);

  struct sockaddr_in their_addr;

  newStream = smalloc(sizeof(connection));
  memset(&their_addr, 0, sizeof(struct sockaddr_in));

  if ((newStream->fd = accept(conn->fd, (struct sockaddr *) & their_addr, &addrlen)) < 0)
  {
    perror("TCP Accept");
    sfree(newStream);
    return 0;
  }

  sprintf(newStream->source, "%s", inet_ntoa(their_addr.sin_addr));

  newStream->read = &cis_tcp_read;
  newStream->write = &cis_tcp_write;
  newStream->close = &cis_tcp_close;

  newStream->recvq = new_buffer_queue(1000);
  newStream->sendq = new_buffer_queue(1000);

  newStream->state.can_shutdown = true;

  /* Make it nonblock and add it to the sockengine */
  flags = fcntl(newStream->fd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(newStream->fd, F_SETFL, flags);

  r_config.socketengine->add(newStream, 1, 0);

  if (conn->callback_accept)
    conn->callback_accept(conn, newStream);

  return 0;
}

void cis_tcp_connected(connection *conn)
{
  // Params are file descriptor, check for readable?, check for writable?
  if (conn->sendq->queue_size != 0)
    r_config.socketengine->mod(conn, 1, 0);
  else
    r_config.socketengine->mod(conn, 1, -1);

  fprintf(stderr, "Connect complete, fd: %d!\n", conn->fd);

  conn->callback_connected(conn);
}

void cis_tcp_connect_failed(connection *conn, int reason)
{
  perror("TCP socket");
  conn->callback_connect_failed(conn, reason);
}

/**
 * @todo Make this properly async!
 */

connection *cis_tcp_connect(char *target_host, int target_port)
{

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
    fprintf(stderr, "TCP Client: Failed to connect to %s:%d ... Unknown host\n", target_host, target_port);
    sfree(conn);
    return NULL;
  }

  sprintf(conn->source, "%s", dns_target->h_addr_list[0]);

  their_addr.sin_family = AF_INET;
  their_addr.sin_port = htons(target_port);    // Short network byte order
  memcpy(&foo, *(dns_target->h_addr_list), sizeof(struct in_addr));
  memcpy(&their_addr.sin_addr.s_addr, * (dns_target->h_addr_list), sizeof(struct in_addr));
  memset(& (their_addr.sin_zero), '\0', 8);      // zero the rest

	free(dns_target);

  fprintf(stderr, "TCP Client: Connecting to %s (%s), port %d\n", target_host, inet_ntoa(foo), target_port);

  if ((conn->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("TCP socket");
    sfree(conn);
    return NULL;
  }

  // Make it nonblock, connect and add it to the sockengine
  flags = fcntl(conn->fd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(conn->fd, F_SETFL, flags);

  conn->connected = &cis_tcp_connected;

  conn->connect_failed = &cis_tcp_connect_failed;

  if (connect(conn->fd, (struct sockaddr *) &their_addr, sizeof(struct sockaddr)) == -1)
  {
    switch (errno)
    {
      case EINPROGRESS: // This is fine, it means it's going to get back to us...
        break;

      default:
        perror("TCP connect");
        sfree(conn);
        return NULL;
    }
  }

  r_config.socketengine->add(conn, 0, 1);

  fprintf(stderr, "Connect started, fd: %d!\n", conn->fd);

  conn->read = &cis_tcp_read;
  conn->write = &cis_tcp_write;
  conn->close = &cis_tcp_close;

  conn->recvq = new_buffer_queue(1000);
  conn->sendq = new_buffer_queue(1000);

  conn->state.can_shutdown = 1;

  return conn;
}

/**
 * This buffer is used as a temp read in.
 * It's 10k big ... if you try and accept lines bigger than that, I will have to hurt you.
 */
char conn_read_in_temp[10000];

int cis_tcp_read(connection *cn)
{
  int nbytes = 0;

  if (cn->state.remote_closed)
    return 0;

  if ((nbytes = recv(cn->fd, conn_read_in_temp, 10000, 0)) <= 0)
  {
    if (nbytes == 0)
    {
      /* Connection closed remotely, so we can't send anymore */
      buffer_empty(cn->sendq);
      buffer_eof(cn->recvq);
      cn->state.remote_closed = 1;
      cis_reap_connection(cn);
      return 0;
    }
    else if (errno == EAGAIN)
    {
      return 0;
    }

    perror("recv");

    cn->syscall_error = nbytes;
    /* Connection closed remotely, so we can't send anymore */
    buffer_empty(cn->sendq);
    buffer_eof(cn->recvq);
    cn->state.remote_closed = 1;
    cis_reap_connection(cn);
    return -1;
  }

  buffer_store(cn->recvq, conn_read_in_temp, nbytes);
  return nbytes;
}

int cis_tcp_write(connection *stream)
{
  /** Bytes sent() */
  int nbytes = 0;
  /** Bytes to send */
  int sendbytes = 0;

  if (stream->state.remote_closed)
    return 0;

  sendbytes = buffer_pop_by_size(stream->sendq, conn_read_in_temp, 5000);

  if (sendbytes == 0)
    return 0;

  if (((nbytes = send(stream->fd, conn_read_in_temp, sendbytes, 0)) <= 0) || (nbytes != sendbytes))
  {
    if ((errno == EAGAIN)||(errno == EWOULDBLOCK))
      return 0;

    perror("send");

    stream->syscall_error = nbytes;
    /* Connection closed remotely, so we can't send anymore */
    buffer_empty(stream->sendq);
    buffer_eof(stream->recvq);
    stream->state.remote_closed = 1;
    cis_reap_connection(stream);

    return -1;
  }

  return nbytes;
}

int cis_tcp_close(connection *stream)
{
  /** This has either been called by the reaper to clean up, or the client to close down... which? */
  if ((stream->state.remote_closed == 0) &&
      ((stream->state.local_read_shutdown == 0) ||
      (stream->state.local_write_shutdown == 0)))
  {
    /** If the remote and local aren't dead, we assume a local close is desired */
    buffer_empty(stream->recvq);
    stream->state.local_read_shutdown = 1;
    stream->state.local_write_shutdown = 1;

    if (stream->callback_close)
      stream->callback_close(stream);

    if (stream->state.can_shutdown == 1)
      shutdown(stream->fd, SHUT_RD);

    cis_reap_connection(stream);

    r_config.socketengine->mod(stream, -1, 0);
  }
  else
  {
    /** If one of them -is- dead, we assume this is the reaper cleaning up, and attempt to shutdown in a clean fashion. */
    /* If state.local_dead is 0, we assume no callback has been fired */
    if (((stream->state.local_read_shutdown == 0) ||
        (stream->state.local_write_shutdown == 0)) &&
        (stream->callback_close))
      stream->callback_close(stream);

    stream->state.remote_closed = 1;

    r_config.socketengine->del(stream);

    close(stream->fd);
  }

  return 0;
}
