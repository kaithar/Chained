/**
 * @file connection.h
 * Chained in Sanity
 * Core types.
 * This file contains the connection struct and it's typedef.
 */

#ifndef _INCLUDES_TYPE_CONNECTION_H_
#define _INCLUDES_TYPE_CONNECTION_H_

typedef struct connection connection;

/**
 * This struct abstracts any sort of connection.
 */
struct connection
{
	/** This should be used to name the connection. */
	char *name;
	int fd;
  
	/** This represents something meaningful to the main app */
	void *data;
  /** This is an extra little flag, just in case you want to use multiple types of socket. */
  int conn_mark;

	/** This struct contains all kinds of usful bit flags.*/
	struct
	{
		/** This signifies that the connection has died and will shortly be reaped. */
		unsigned int dead:1;
		/** This signifies that the connection has been closed */
		unsigned int closed:1;
		/** Socket engine listening for readable. */
		unsigned int listen_read:1;
		/** Socket engine listening for writable. */
		unsigned int listen_write:1;
	} state;
	
	/** This is an SSL pointer.  It's typed void to avoid the dependancy */
	void *ssl;
	/** This is a FILE pointer.  It's typed void to avoid the dependancy */
	void *file;
	
	/**
	 * @name RecvQ
	 */
	/*@{*/
	linklist_root *recvq;			/**< This is a linklist of messages received from this connection but not processed yet. */
	int recvq_size;						/**< This is how much data we're storing on this recvq linklist */
	unsigned char *recvq_buf;	/**< Buffer for storing part read data */
	int recvq_buf_used;				/**< Amount of space used in the buffer. */
	int recvq_buf_free;				/**< Amount of space left in the buffer. */
	/*@}*/
	
	/**
	 * @name SendQ
	 */
	/*@{*/
	linklist_root *sendq;			/**< This is a linklist of messages about to be sent from this connection. */
	int sendq_size;						/**< This is how much data we're storing on this sendq linklist */
	unsigned char *sendq_buf;	/**< Buffer for storing part sent data */
	int sendq_buf_used;				/**< Amount of space used in the buffer. */
	int sendq_buf_free;				/**< Amount of space left in the buffer. */
	/*@}*/
	
	/**
	 * @name IO Functions
	 */
	/*@{*/
	/** This function calls the function doing the writing */
	int (*write)(connection *, char *);
	/** This function calls the function doing the reading 
	 * @note: If this connection is accepting connections, this is where it is done.
	 */
	int (*read)(connection *, int how_much, char *buffer);
	/** This function should close down the connection */
	int (*close)(connection *);
	/*@}*/

	/** Shutdown the encryption */
	int (*enc_close) (connection *);

	/**
	 * @name Events callbacks...
	 */
	/*@{*/
	/** Line read in... */
	int (*callback_read) (connection *, unsigned char *);
	/** New connection accepted! */
	int (*callback_accept) (connection *on, connection *newcn);
	/** Connection closed */
	int (*callback_close)(connection *);
	/*@}*/
};

#endif
