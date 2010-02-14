#include "common.h"

static void
Port_dealloc(PyObject *s)
{
    pyPort *self = (pyPort *)s;
	if (self->conn != NULL)
	{
		// Shouldn't be possible, but oh well :/
		self->conn->close(self->conn);
	}
    Py_XDECREF(self->host);
    Py_XDECREF(self->factory);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
Port_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyPort *self;

    self = (pyPort *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->host = PyString_FromString("0.0.0.0");
        if (self->host == NULL)
          {
            Py_DECREF(self);
            return NULL;
          }

        Py_INCREF(Py_None);
        self->factory = Py_None;
        self->port = 0;
    }

    return (PyObject *)self;
}

static int
Port_init(pyPort *self, PyObject *args, PyObject *kwds)
{
    PyObject *name=NULL, *host=NULL, *tmp, *factory=NULL;

    static char *kwlist[] = {"name", "host", "port", "factory", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "SSiO", kwlist, 
                                      &name, &host, 
                                      &self->port, &factory))
        return -1; 

    if (host) {
        tmp = self->host;
        Py_INCREF(host);
        self->host = host;
        Py_XDECREF(tmp);
    }

    if (factory) {
        tmp = self->factory;
        Py_INCREF(factory);
        self->factory = factory;
        Py_XDECREF(tmp);
    }

    self->conn = ipv4_tcp_listen((name?PyString_AsString(name):"Listening port"),PyString_AsString(self->host),self->port);
    self->conn->data = self;
    self->conn->callback_accept = &accept_callback;
    self->conn->callback_close = &port_close_callback;

    return 0;
}


static PyMemberDef Port_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef Port_methods[] = {
    {NULL}  /* Sentinel */
};


PyTypeObject pyPortType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "chained.primative.net.Port",             /*tp_name*/
    sizeof(pyPort),            /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    Port_dealloc,              /*tp_dealloc*/
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
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /*tp_flags*/
    "Port object",             /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    Port_methods,             /* tp_methods */
    Port_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Port_init,      /* tp_init */
    0,                         /* tp_alloc */
    Port_new,                 /* tp_new */
};
