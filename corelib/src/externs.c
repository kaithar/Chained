
#include <includes/chained.h>

/**
 * @brief I am set by config param "debug"
 * @internal
 * @todo This should probably be adjusted to a bitmask or something
 */

int DEBUG = 0; // Set by the config keyword "debug" ... no params


/**
 * @brief Connection tracking...
 * @internal
 * @todo FIXME: Is this still needed or can it be removed?
 */
connection *connections[__MAXFDS__];

reactor_config r_config;