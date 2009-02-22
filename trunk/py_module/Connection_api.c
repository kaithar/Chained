#include "common.h"

int Connection_read_callback(connection *cn, unsigned char *line)
{
	PyObject *tmp;
	pyConnection *self = (pyConnection *)cn->data;
	tmp = PyObject_CallMethodObjArgs(self->protocol, PyString_FromString("onRead"), self, PyString_FromString(line), NULL);
	Py_XDECREF(tmp);
}

int Connection_close_callback(connection *cn)
{
	PyObject *tmp;
	pyConnection * self = (pyConnection *)cn->data;
	tmp = PyObject_CallMethodObjArgs(self->protocol, PyString_FromString("onClose"), self, NULL);
	Py_XDECREF(tmp);
	self->conn = NULL;
	Py_XDECREF(self);
}


PyObject* Connection_write(pyConnection *self, PyObject *args)
{
	char *msg;
	if (!PyArg_ParseTuple(args, "s", &msg))
		return NULL;
	CONN_ASSERT(self->conn, NULL);
	cprintf(self->conn, "%s", msg);
	Py_RETURN_NONE;
}
