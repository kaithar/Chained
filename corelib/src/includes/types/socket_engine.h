/**
 * @file socket_engine.h
 * This header handles -all- socket engines.
 * A new socket engine implementation should be extracted to this interface.
 */

#ifndef __socketengine_include__
#define __socketengine_include__

/**
 * Socket engine abstraction.
 */
typedef struct
{
	int fdcount; /**< How many connections we're listening to. */
	connection *cns[__MAXFDS__]; /**< Our listening connection collection. */
	int (*add)(connection *cn, int addread, int addwrite); /**< Add a new connection to the engine. */
	/**
	 * Modifies the socket monitering conditions.
	 * For mod_read/mod_write, pass:
	 * - \<0 to remove that flag
	 * - 0 for no change
	 * - \>1 to add that flag
	 * 
	 * So to remove read polling and add write polling to conn, call:
	 * @code
	 * socketengine->mod(conn,-1,1);
	 */
	int (*mod)(connection *cn, int mod_read, int mod_write); 
	int (*del)(connection *cn); /**< Remove a connection from the engine. */
	int (*wait)(connection *read_events[], connection *write_events[], int howlong); /**< Wait for events. */
} socket_engine;

#endif
