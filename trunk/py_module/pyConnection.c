#include "common.h"

void Connection_dealloc(PyObject *s)
{
	pyConnection *self = (pyConnection *)s;
	if (self->conn != NULL)
	{
		// Shouldn't be possible, but oh well :/
		self->conn->close(self->conn);
		self->conn->data = NULL;
	}
	Py_XDECREF(self->protocol);
	self->ob_type->tp_free((PyObject*)self);
}

/* Stubs */

PyObject* Connection_new(PyTypeObject *type, PyObject *args, PyObject *kwds) { return NULL; }
int Connection_init(pyConnection *self, PyObject *args, PyObject *kwds) { return 0; }

/* Structs */

PyMemberDef Connection_members[] = {
    {NULL}  /* Sentinel */
};

PyMethodDef Connection_methods[] = {
    {"write", (binaryfunc)Connection_write, METH_VARARGS, "Write data to the connection"},
    {NULL}  /* Sentinel */
};

static PyGetSetDef Connection_getseters[] = {
    {"protocol", (getter)Connection_getprotocol, (setter)Connection_setprotocol, "Connection protocol", NULL},
    {"fd", (getter)Connection_getfd, (setter)Connection_ro, "Connection file descriptor", NULL},
    {"source", (getter)Connection_getsource, (setter)Connection_ro, "Connection remote host", NULL},
    {NULL}  /* Sentinel*/
};

PyTypeObject pyConnectionType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "chained.primative.net.Connection",      /*tp_name*/
    sizeof(pyConnection),      /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    Connection_dealloc,        /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Connection object",       /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    Connection_methods,        /* tp_methods */
    Connection_members,        /* tp_members */
    Connection_getseters,      /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Connection_init, /* tp_init */
    0,                         /* tp_alloc */
    Connection_new,            /* tp_new */
};
