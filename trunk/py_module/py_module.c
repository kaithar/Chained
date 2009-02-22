#include "common.h"

static PyObject *
chained_run(PyObject *self, PyObject *args)
{
    cis_run();
    Py_RETURN_NONE;
}

static PyObject *
chained_connect(PyObject *self, PyObject *args)
{
	char *name, *host;
	int port;
	pyConnection *ncn;
	connection *cn;
	if (!PyArg_ParseTuple(args, "ssi", &name, &host, &port))
		return NULL;

	cn = ipv4_tcp_connect(name, host, port);
	if (cn == NULL)
	{
		PyErr_SetString(PyExc_IOError, "Unable to create connection");
		return NULL;
	}
	cn->callback_read = &Connection_read_callback;
	cn->callback_close = &Connection_close_callback;
	ncn = (pyConnection *)pyConnectionType.tp_alloc(&pyConnectionType, 0);
	
	if (ncn != NULL) {
		Py_INCREF(Py_None);
		ncn->protocol = Py_None;
	}

	ncn->conn = cn;
	cn->data = ncn;
	return (PyObject *)ncn;
}


static PyMethodDef chained_methods[] = {
    {"run", chained_run, METH_NOARGS, "Start the reactor"},
    {"connect", chained_connect, METH_VARARGS, "Make a tcp connection"},
    {NULL}  /* Sentinel */
};

PyOS_sighandler_t old_sigint;

void int_handler(int i)
{
	cis_kill_reactor();
	old_sigint(i);
}

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initnet(void) 
{
    PyObject* m;

    cis_init();
    cis_load_selectengine();

    if (PyType_Ready(&pyPortType) < 0)
        return;

    if (PyType_Ready(&pyConnectionType) < 0)
        return;

    m = Py_InitModule3("chained.net", chained_methods,
                       "Network communications library.");

    Py_INCREF(&pyPortType);
    PyModule_AddObject(m, "Port", (PyObject *)&pyPortType);
    Py_INCREF(&pyConnectionType);
    PyModule_AddObject(m, "Connection", (PyObject *)&pyConnectionType);

    old_sigint = PyOS_setsig(SIGINT, &int_handler);
}
