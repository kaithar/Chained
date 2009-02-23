#import "common.h"

int port_close_callback(connection *cn)
{
	PyObject *tmp;
	pyPort * self = (pyPort *) cn->data;
	tmp = PyObject_CallMethodObjArgs(self->factory, PyString_FromString("onClose"), self, NULL);
	if ((tmp == NULL) && (PyErr_Occurred() != NULL))
		PyErr_Print();
	Py_XDECREF(tmp);
	self->conn = NULL;
	Py_XDECREF(self);
}

int accept_callback (connection *parent, connection *cn)
{
	PyObject *tmp;
	pyConnection *ncn;

	cn->callback_read = &Connection_read_callback;
	cn->callback_close = &Connection_close_callback;

	ncn = (pyConnection *)pyConnectionType.tp_alloc(&pyConnectionType, 0);
	
	if (ncn != NULL) {
		Py_INCREF(Py_None);
		ncn->protocol = Py_None;
	}

	ncn->conn = cn;
	cn->data = ncn;

	tmp = PyObject_CallMethodObjArgs(((pyPort *)parent->data)->factory, PyString_FromString("onAccept"), ((pyPort *)parent->data), ncn, NULL);
	if ((tmp == NULL) && (PyErr_Occurred() != NULL))
		PyErr_Print();
	Py_XDECREF(tmp);
}
