/* Includes */
#include <Python.h>
#include "structmember.h"

#include "chained.h"

/* Defines */

#define CONN_ASSERT(cn, r) \
	if (cn == NULL) \
	{ \
		PyErr_SetString(PyExc_IOError, "That connection has been closed"); \
		return r; \
	}


/* Connection.c */
typedef struct {
    PyObject_HEAD;
    /* Type-specific fields go here. */
    connection *conn;
    PyObject *protocol;
} pyConnection;

extern PyTypeObject pyConnectionType;

/* Port.c */

typedef struct {
    PyObject_HEAD;
    /* Type-specific fields go here. */
    PyObject *host;
    int port;
    connection *conn;
    PyObject *factory;
} pyPort;

extern PyTypeObject pyPortType;

/* Connection_api.c */

extern PyObject* Connection_write(pyConnection *self, PyObject *args);

extern int Connection_read_callback(connection *cn, unsigned char *line);
extern int Connection_close_callback(connection *cn);

/* Connection_getset_funcs.c */
extern int Connection_ro(pyConnection *self, PyObject *value, void *closure);
extern PyObject *Connection_getprotocol(pyConnection *self, void *closure);
extern int Connection_setprotocol(pyConnection *self, PyObject *value, void *closure);
extern PyObject* Connection_getfd(pyConnection *self, void *closure);
extern PyObject* Connection_getsource(pyConnection *self, void *closure);

/* Port_api.c */

extern int port_close_callback(connection *cn);
extern int accept_callback (connection *parent, connection *cn);
