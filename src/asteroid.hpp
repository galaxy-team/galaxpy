#include <Python.h>
#include <structmember.h>

typedef struct {
    PyObject_HEAD

    /**
     * Dictionary mapping labels to declaration points
     */
    PyObject *exported_labels;

    /**
     * Set containing positions where non-imported labels are used
     */
    PyObject *used_labels;

    /**
     * Dictionary mapping positions to labels used in those positions
     */
    PyObject *imported_labels;

    /**
     * The machine code
     */
    PyObject *object_code;
} asteroid;

static void
asteroid_dealloc(asteroid* self)
{
    Py_XDECREF(self->exported_labels);
    Py_XDECREF(self->used_labels);
    Py_XDECREF(self->imported_labels);
    Py_XDECREF(self->object_code);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject*
asteroid_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    asteroid *self;

    self = (asteroid *)type->tp_alloc(type, 0);
    if(self != NULL) {
        self->exported_labels = PyDict_New();
        if(self->exported_labels == NULL) {
            Py_DECREF(self);
            return NULL;
        }

        self->used_labels = PySet_New(0);
        if(self->used_labels == NULL) {
            Py_DECREF(self);
            return NULL;
        }

        self->imported_labels = PyDict_New();
        if(self->imported_labels == NULL) {
            Py_DECREF(self);
            return NULL;
        }

        self->object_code = PyList_New(0);
        if(self->object_code == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }

    return (PyObject *)self;

}

static int
asteroid_init(asteroid *self, PyObject *args, PyObject *kwds)
{
    PyObject *exported_labels=NULL, *used_labels=NULL,
             *imported_labels=NULL, *object_code=NULL, *tmp;

    static char *kwlist[] = {
        "exported_labels", "used_labels",
        "imported_labels", "object_code",
        NULL
    };

    if(! PyArg_ParseTupleAndKeywords(args, kwds, "|OOOO", kwlist,
                                     &exported_labels, &used_labels,
                                     &imported_labels, &object_code))
        return -1;

    if(exported_labels) {
        tmp = self->exported_labels;
        Py_INCREF(exported_labels);
        self->exported_labels = exported_labels;
        Py_XDECREF(tmp);
    }


    if(used_labels) {
        tmp = self->used_labels;
        Py_INCREF(used_labels);
        self->used_labels = used_labels;
        Py_XDECREF(tmp);
    }


    if(imported_labels) {
        tmp = self->imported_labels;
        Py_INCREF(imported_labels);
        self->imported_labels = imported_labels;
        Py_XDECREF(tmp);
    }


    if(object_code) {
        tmp = self->object_code;
        Py_INCREF(object_code);
        self->object_code = object_code;
        Py_XDECREF(tmp);
    }

    return 0;
}

static PyMemberDef asteroid_members[] = {
    {const_cast<char *>("exported_labels"), T_OBJECT_EX, offsetof(asteroid, exported_labels), 0,
     const_cast<char *>("Dictionary mapping labels to declaration points")},
    {const_cast<char *>("used_labels"), T_OBJECT_EX, offsetof(asteroid, used_labels), 0,
     const_cast<char *>("List of positions where non-imported labels are used")},
    {const_cast<char *>("imported_labels"), T_OBJECT_EX, offsetof(asteroid, imported_labels), 0,
     const_cast<char *>("Dictionary mapping positions to labels used in those positions")},
    {const_cast<char *>("object_code"), T_OBJECT_EX, offsetof(asteroid, object_code), 0,
     const_cast<char *>("The machine code")},
    {NULL}  /* Sentinel */
};

static PyTypeObject asteroid_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "galaxy.asteroid",        /* tp_name */
    sizeof(asteroid),                 /* tp_basicsize */
    0,                                /* tp_itemsize */
    (destructor)asteroid_dealloc,     /* tp_dealloc */
    0,                                /* tp_print */
    0,                                /* tp_getattr */
    0,                                /* tp_setattr */
    0,                                /* tp_reserved */
    0,                                /* tp_repr */
    0,                                /* tp_as_number */
    0,                                /* tp_as_sequence */
    0,                                /* tp_as_mapping */
    0,                                /* tp_hash  */
    0,                                /* tp_call */
    0,                                /* tp_str */
    0,                                /* tp_getattro */
    0,                                /* tp_setattro */
    0,                                /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,          /* tp_flags */
    "Object file objects",            /* tp_doc */
    0,                                /* tp_traverse */
    0,                                /* tp_clear */
    0,                                /* tp_richcompare */
    0,                                /* tp_weaklistoffset */
    0,                                /* tp_iter */
    0,                                /* tp_iternext */
    0,                                /* tp_methods */
    asteroid_members,                 /* tp_members */
    0,                                /* tp_getset */
    0,                                /* tp_base */
    0,                                /* tp_dict */
    0,                                /* tp_descr_get */
    0,                                /* tp_descr_set */
    0,                                /* tp_dictoffset */
    (initproc)asteroid_init,          /* tp_init */
    0,                                /* tp_alloc */
    asteroid_new,                     /* tp_new */
};


static struct PyModuleDef asteroidmodule = {
    PyModuleDef_HEAD_INIT,
    "asteroid",    // name of module
    "Module that provides a Python version of Galaxy's asteroid",
    -1,            // module keeps state in global variables
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_asteroid(void)
{
    PyObject* m;

    asteroid_type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&asteroid_type) < 0) {
        return NULL;
    }

    m = PyModule_Create(&asteroidmodule);
    if (m == NULL) {
        return NULL;
    }

    Py_INCREF(&asteroid_type);
    if (PyModule_AddObject(m, "Asteroid", (PyObject *)&asteroid_type) < 0) {
        return NULL;
    }

    return m;
}
