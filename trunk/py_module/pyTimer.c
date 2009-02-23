#include "common.h"

static void
Timer_dealloc(PyObject *s)
{
    pyTimer *self = (pyTimer *)s;
	if (self->ev != NULL)
	{
		// Shouldn't be possible, but oh well :/
		timers_del(self->ev);
		free(self->ev);
	}
    Py_XDECREF(self->callback);
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
Timer_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    pyTimer *self;

    self = (pyTimer *)type->tp_alloc(type, 0);
    if (self != NULL) {
        Py_INCREF(Py_None);
        self->callback = Py_None;
        self->ev = NULL;
    }

    return (PyObject *)self;
}

static int Timer_callback(timer_event *ev, void *c)
{
	pyTimer *self = c;
	PyObject *tmp;
	tmp = PyObject_CallMethodObjArgs(self->callback, PyString_FromString("onTimer"), self, NULL);
	if ((tmp == NULL) && (PyErr_Occurred() != NULL))
		PyErr_Print();
	Py_XDECREF(tmp);
	if (ev->repetitions == 1)
	{
		self->ev = NULL;
		Py_XDECREF(self);
	}
}

static int
Timer_init(pyTimer *self, PyObject *args, PyObject *kwds)
{
    PyObject *tmp, *callback=NULL;
		timer_event *ev = smalloc(sizeof(timer_event));
		
    static char *kwlist[] = {"delay", "repeat", "callback", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "iiO", kwlist, 
                                      &ev->delay, &ev->repetitions, 
                                      &callback))
        return -1; 

    if (callback) {
        tmp = self->callback;
        Py_INCREF(callback);
        self->callback = callback;
        Py_XDECREF(tmp);
    }

		ev->when = time(0)+ev->delay;
		ev->callback = &Timer_callback;
		ev->callback_param = (void *)self;
		self->ev = ev;
		timers_add(ev);

    return 0;
}


static PyMemberDef Timer_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef Timer_methods[] = {
    {NULL}  /* Sentinel */
};

PyTypeObject pyTimerType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "chained.Timer",             /*tp_name*/
    sizeof(pyTimer),            /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    Timer_dealloc,              /*tp_dealloc*/
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
    "Timer object",             /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    Timer_methods,             /* tp_methods */
    Timer_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Timer_init,      /* tp_init */
    0,                         /* tp_alloc */
    Timer_new,                 /* tp_new */
};
