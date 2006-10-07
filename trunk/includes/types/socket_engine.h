/**
 * @file socket_engine.h
 * This header handles -all- socket engines.
 * A new socket engine implementation should be extracted to this interface.
 */

#ifndef __socketengine_include__
#define __socketengine_include__

typedef struct socket_engine socket_engine;

/**
 * Socket engine abstraction.
 */
struct socket_engine
{
	int fdcount; /**< How many connections we're listening to. */
	connection *cns[__MAXFDS__]; /**< Our listening connection collection. */
	int (*add)(connection *cn, int addread, int addwrite); /**< Add a new connection to the engine. */
	int (*del)(connection *cn); /**< Remove a connection from the engine. */
	int (*wait)(connection *read_events[], connection *write_events[]); /**< Wait for events. */
};

#endif
