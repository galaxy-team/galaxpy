#include <Python.h>
#include <structmember.h>

struct object_file {
    PyObject_HEAD

    /**
     * Dictionary mapping labels to declaration points
     */
    PyObject *exported_labels;

    /**
     * List of positions where non-imported labels are used
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
};

static void
object_file_dealloc(object_file* self)
{
    Py_XDECREF(self->exported_labels);
    Py_XDECREF(self->used_labels);
    Py_XDECREF(self->imported_labels);
    Py_XDECREF(self->object_code);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject*
object_file_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    object_file *self;

    self = (object_file *)type->tp_alloc(type, 0);
    if(self != NULL) {
        self->exported_labels = PyDict_New();
        if(self->exported_labels == NULL) {
            Py_DECREF(self);
            return NULL;
        }

        self->used_labels = PyList_New(0);
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
object_file_init(object_file *self, PyObject *args, PyObject *kwds)
{
    PyObject *exported_labels=NULL, *used_labels=NULL,
             *imported_labels=NULL, *object_code=NULL, *tmp;

    static const char *kwlist[] = {"exported_labels", "used_labels",
                                   "imported_labels", "object_code",
                                   NULL};

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

static PyMemberDef object_file_members[] = {
    {const_cast<char *>("exported_labels"), T_OBJECT_EX, offsetof(object_file, exported_labels), 0,
     const_cast<char *>("Dictionary mapping labels to declaration points")},
    {const_cast<char *>("used_labels"), T_OBJECT_EX, offsetof(object_file, used_labels), 0,
     const_cast<char *>("List of positions where non-imported labels are used")},
    {const_cast<char *>("imported_labels"), T_OBJECT_EX, offsetof(object_file, imported_labels), 0,
     const_cast<char *>("Dictionary mapping positions to labels used in those positions")},
    {const_cast<char *>("object_code"), T_OBJECT_EX, offsetof(object_file, object_code), 0,
     const_cast<char *>("The machine code")},
    {NULL}  /* Sentinel */
};

static PyTypeObject object_file_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "galaxy.jupiter.object_file",     /* tp_name */
    sizeof(object_file),              /* tp_basicsize */
    0,                                /* tp_itemsize */
    (destructor)object_file_dealloc,  /* tp_dealloc */
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
    object_file_members,              /* tp_members */
    0,                                /* tp_getset */
    0,                                /* tp_base */
    0,                                /* tp_dict */
    0,                                /* tp_descr_get */
    0,                                /* tp_descr_set */
    0,                                /* tp_dictoffset */
    (initproc)object_file_init,       /* tp_init */
    0,                                /* tp_alloc */
    object_file_new,                  /* tp_new */
};
