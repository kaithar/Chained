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

  /** If this connection uses an FD, it's stored here. */
  int fd;

  /** This is an SSL pointer.  It's typed void to avoid the dependancy */
  void *ssl;
  /** This is a FILE pointer.  It's typed void to avoid the dependancy */
  void *file;

  /** This contains some sort of meaningful source string... like an ip */
  char source[256];

  /** This represents something meaningful to the main app */
  void *data;

  /** This is an extra little flag, just in case you want to use multiple types of socket. */
  int conn_mark;

  /** This contains the error that killed the connection */
  int syscall_error;

  /** If this connection encapsulates another, it's stored here. */
  connection *encapsulates;

  /** If this connection is encapsulated by anther, it's stored here. */
  connection *encapsulated_by;

  /**
    * This struct contains all kinds of useful bit flags
    * Connsider these flags read only, fiddling will probably break things.
    */
  struct
  {

    /**
      * Connecting
      */
    unsigned int connecting:1;

    /**
      * This signifies that the connection has been added to the reaper.
      */
    unsigned int reaping:1;

    /**
      * This signifies that the connection has encountered a fatal error when reading/writing.
      * Recvq will continue to be processed.  Sendq will be silently discarded.
      */
    unsigned int remote_closed:1;

    /**
      * This signifies the local connection has shutdown() the incoming channel.
      * Sendq will continue to be processed.  Readq will be silently discarded.
      */
    unsigned int local_read_shutdown:1;

    /**
      * This signifies the local connection has shutdown() the outgoing channel.
      * Recvq will continue to be processed.  Sendq will be silently discarded.
      */
    unsigned int local_write_shutdown:1;

    /** This is used to indicate a connection may be allowed to use shutdown */
    unsigned int can_shutdown:1;

    /** Socket engine listening for readable. */
    unsigned int listen_read:1;

    /** Socket engine listening for writable. */
    unsigned int listen_write:1;

  } state;

  /**
    * @name Packet queues
    */
  buffer_queue *recvq;
  buffer_queue *sendq;

  /**
    * @name IO Functions
    */
  /*@{*/

  /** This function is responsible for doing anything that needs doing just after connection */
  void (*connected)(connection *);

  /** This function is responsible for doing anything that needs to happen if the connection fails */
  void (*connect_failed)(connection *, int);

  /** This function calls the function doing the writing, data assumed to be in sendq */
  int (*write)(connection *);

  /** This function calls the function doing the reading, data stored in recvq
    * @note: If this connection is accepting connections, this is where it is done.
    */
  int (*read)(connection *);

  /** Shutdown() */
  int (*shutdown)(connection *, int);

  /** This function should close down the connection */
  int (*close)(connection *);

  /*@}*/

  /**
    * @name Events callbacks...
    */
  /*@{*/

  /** This function is responsible for doing anything that needs doing just after connection */
  void (*callback_connected)(connection *);

  /** This function is responsible for doing anything that needs to happen if the connection fails */
  void (*callback_connect_failed)(connection *, int);

  /** Data received... */
  int (*callback_read) (connection *);

  /** New connection accepted! */
  int (*callback_accept) (connection *on, connection *newcn);

  /** Remote connection lost */
  int (*callback_remote_lost) (connection *);

  /** Local shutdown() */
  int (*callback_shutdown) (connection *, int);

  /** Connection closed */
  int (*callback_close)(connection *);
  /*@}*/
};

#endif
