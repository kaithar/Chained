/**
 * @file openssl.c
 * @brief OpenSSL support
 * Chained in Sanity
 * This module abstracts the stuff that is particular to OpenSSL
 * CHAINED: IGNORE
 */

/* Some ssl headers */
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <fcntl.h> // O_NONBLOCK F_GETFL F_SETFL

#include "chained.h"

/*-----------------------------------*/
#define KEY_PEM "conf/key.pem"
#define CERT_PEM "conf/cert.pem"
#define CA_PEM "conf/ca.pem"

SSL_CTX *ssl_client_ctx;	/* SSL context */
SSL_CTX *ssl_server_ctx;	/* SSL server context */

/*-----------------------------------*/

static int my_ssl_verify_cb(int preverify, X509_STORE_CTX * ctxs)
{
	return 1;
}

/*-----------------------------------*/
bool cis_openssl_init()
{
	static int inited = 0;
	if (inited == 1)
		return;

	/* Init SSL */
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	SSL_library_init();

	ssl_client_ctx = SSL_CTX_new(SSLv23_client_method());	/* Init context */

	if (ssl_client_ctx == NULL)
	{
		ERR_print_errors_fp(stdout);
		printf("Failed to create SSL Client Context ... Fatal error.\n");
		exit(1);
	}
	
	ssl_server_ctx = SSL_CTX_new(SSLv23_server_method());	/* Init server context */

	if (ssl_server_ctx == NULL)
	{
		ERR_print_errors_fp(stdout);
		printf("Failed to create SSL Server Context ... Fatal error.\n");
		exit(1);
	}

	/*---*/

	if (!SSL_CTX_use_certificate_chain_file(ssl_client_ctx, CERT_PEM))
		printf("Failed to load ssl client certicate, attempting to continue...\n");

	if (!SSL_CTX_use_PrivateKey_file(ssl_client_ctx, KEY_PEM, SSL_FILETYPE_PEM))
		printf("Failed to load ssl client key, attempting to continue...\n");

	if (!SSL_CTX_load_verify_locations(ssl_client_ctx, CA_PEM, 0))
		printf("Failed to load ssl client CA file, attempting to continue...\n");

	/*---*/

	if (!SSL_CTX_use_certificate_chain_file(ssl_server_ctx, CERT_PEM))
		printf("Failed to load ssl server certicate, attempting to continue...\n");

	if (!SSL_CTX_use_PrivateKey_file(ssl_server_ctx, KEY_PEM, SSL_FILETYPE_PEM))
		printf("Failed to load ssl server key, attempting to continue...\n");

	if (!SSL_CTX_load_verify_locations(ssl_server_ctx, CA_PEM, 0))
		printf("Failed to load ssl server CA file, attempting to continue...\n");

	/*---*/

	SSL_CTX_set_verify(ssl_server_ctx, SSL_VERIFY_PEER | SSL_VERIFY_CLIENT_ONCE, &my_ssl_verify_cb);
	SSL_CTX_set_verify_depth(ssl_server_ctx, 0);
	SSL_CTX_set_session_cache_mode(ssl_server_ctx, SSL_SESS_CACHE_OFF);

	inited = 1;
	return true;
}



/*-----------------------------------*/
void cis_openssl_port_upgrade(connection *on)
{
	cis_openssl_init();
	on->enc_accept = &cis_openssl_accept;
}

int cis_openssl_accept(connection *conn)
{
	int i, j = 0;
	cis_openssl_init();

	if ((conn->ssl = SSL_new(ssl_server_ctx)) == NULL)
	{
		//exit_client(conn->client, "SSL_new failed");
		printf("SSL_new failed!\n");
		return -1;
	}

	if (SSL_set_fd(conn->ssl, conn->fd) == 0)
	{
		//exit_client(conn->client, ERR_error_string(SSL_get_error(conn->ssl, 0), NULL));
		printf("SSL_set_fd failed!\n");
		return -1;
	}
	
	SSL_set_accept_state(conn->ssl);
	
	if ((i = SSL_accept(conn->ssl)) <= 0)
	{
		j = SSL_get_error(conn->ssl, i);
	
		if ((j == SSL_ERROR_WANT_READ) || (j == SSL_ERROR_WANT_WRITE))
			;	/* Listening SSL_accept should have blocked! */
		else
			printf("Listening SSL_accept failed! %d: %s\n", j, ERR_error_string(j, NULL));
	}

	conn->enc_close = &cis_openssl_close;
	conn->read = &cis_openssl_read;
	conn->write = &cis_openssl_write;
	conn->state.can_shutdown = 0;

	return 0;
}

/**
 * This works on -outgoing- connections ... hence the client bit.
 * That means you are -requesting- ssl, while the accept function above is -accepting- ssl
 * FIXME: This needs to be made async...
 */
int cis_openssl_client_upgrade(connection *stream)
{
  int i,foo = 0;
  int flags = 0;

  printf("SSL: Upgrading connection\n");
	cis_openssl_init();

  /* Make it ssl */
	if ((stream->ssl = SSL_new(ssl_client_ctx)) == NULL)
	{
		printf("SSL_new failed!\n");
		return -1;
	}
	
  SSL_set_fd(stream->ssl, stream->fd);
  SSL_set_connect_state(stream->ssl);

  /* HACK!!! */
  flags = fcntl(stream->fd,F_GETFL, 0);
  flags &= ~O_NONBLOCK;
  fcntl(stream->fd,F_SETFL,flags);
  /* ---- */

  if ((i = SSL_connect(stream->ssl)) <= 0) {
		foo = SSL_get_error(stream->ssl,i);
    printf("Upgrading SSL_connect failed! %d: %s\n", foo, ERR_error_string(foo, NULL));
  }

  /* HACK!!! */
  flags = fcntl(stream->fd,F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(stream->fd,F_SETFL,flags);
  /* ---- */

  stream->write = &cis_openssl_write;
  stream->read = &cis_openssl_read;
  stream->enc_close = &cis_openssl_close;
	stream->state.can_shutdown = 0;
  printf("SSL: Upgrade complete\n");
  return 0;
}

int cis_openssl_write(connection *stream, char *str)
{
	int nbytes = 0;

	if ((nbytes = SSL_write(stream->ssl, str, strlen(str))) == -1)
	{
		perror("ssl_write");	/* TODO: Better error handling needed here! */
	}
	
	return nbytes;
}

int cis_openssl_read(connection *cn, int how_much, char *buffer)
{
	int nbytes = 0;
	int foo = 0;
	
	if (cn->state.remote_dead)
	{
		*buffer = '\0';
		return 0;
	}
	
	if ((nbytes = SSL_read(cn->ssl, buffer, how_much-1)) <= 0)
	{
		if (nbytes == 0)
		{
			/* Connection closed remotely, so we can't send anymore */
			discard_sendq(cn);
			cn->state.remote_dead = 1;
			if (cn->recvq->members == 0)
				cis_reap_connection(cn);
			return 0;
		}
		else if ((foo = SSL_get_error(cn->ssl, nbytes)) == SSL_ERROR_WANT_READ)
		{  /* SSL equiv of EAGAIN ;) */
			return 0;
		}
		else if (foo == SSL_ERROR_WANT_WRITE)
		{
			return 0;
		}
		else
		{
			perror("Underlying error");
			printf("SSL Read Error: %d: %s\n", foo, ERR_error_string(foo, NULL));
		}
			
		cn->syscall_error = nbytes;
		/* Connection closed remotely, so we can't send anymore */
		discard_sendq(cn);
		cn->state.remote_dead = 1;
		if (cn->recvq->members == 0)
			cis_reap_connection(cn);
		return -1;
	}

	buffer[nbytes] = '\0';
	return nbytes;
}

int cis_openssl_close(connection *stream)
{
	return SSL_shutdown(stream->ssl);
}
