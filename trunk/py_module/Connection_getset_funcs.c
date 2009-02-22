#include "common.h"

int Connection_ro(pyConnection *self, PyObject *value, void *closure)
{
  PyErr_SetString(PyExc_TypeError, "Cannot set a read only attribute");
  return -1;
}

/* Connection.protocol */

PyObject *Connection_getprotocol(pyConnection *self, void *closure)
{
    Py_INCREF(self->protocol);
    return self->protocol;
}

int Connection_setprotocol(pyConnection *self, PyObject *value, void *closure)
{
  if (value == NULL) {
    PyErr_SetString(PyExc_TypeError, "Cannot delete the protocol attribute");
    return -1;
  }
  Py_CLEAR(self->protocol);
  Py_INCREF(value);
  self->protocol = value;

  return 0;
}

/* Connection.fd */

PyObject* Connection_getfd(pyConnection *self, void *closure)
{
	CONN_ASSERT(self->conn, NULL);
	return PyInt_FromLong(self->conn->fd);
}

/* Connection.source */

PyObject* Connection_getsource(pyConnection *self, void *closure)
{
	CONN_ASSERT(self->conn, NULL);
	return PyString_FromString(self->conn->source);
}
