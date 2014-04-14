/*

This file is part of galaxpy.

galaxpy is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

galaxpy is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with galaxpy.  If not, see <http://www.gnu.org/licenses/>.

Your copy of the GNU Lesser General Public License should be in the
file named "LICENSE-LGPL.txt".

*/

#include <Python.h>
#include <structmember.h>

#include <libsaturn.hpp>
#include <invalid_opcode.hpp>
#include <queue_overflow.hpp>

#include "pydevice.hpp"

static PyObject *InvalidOpcodeError;
static PyObject *QueueOverflowError;

struct Device {
    PyObject_HEAD

    /// the device the object is wrapping
    galaxy::saturn::device* hw;
};

static void
Device_dealloc(Device* self)
{
    delete self->hw;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
Device_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Device *self;

    self = (Device *)type->tp_alloc(type, 0);

    return (PyObject *)self;
}

static int
Device_init(Device *self, PyObject *args, PyObject *kwds)
{
    // init the device here because of magic
    self->hw = (galaxy::saturn::device *)new PyDevice((PyObject *)self);
    return 0;
}

static PyObject *
Device_getid(Device *self, void *closure)
{
    PyObject *id = PyLong_FromLong(self->hw->id);
    if (id == NULL) {
        return NULL;
    }

    return id;
}

static int
Device_setid(Device *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->hw->id = PyLong_AsLong(value);

    return 0;
}

static PyObject *
Device_getmanufacturer(Device *self, void *closure)
{
    PyObject *manufacturer = PyLong_FromLong(self->hw->manufacturer);
    if (manufacturer == NULL) {
        return NULL;
    }

    return manufacturer;
}

static int
Device_setmanufacturer(Device *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->hw->manufacturer = PyLong_AsLong(value);

    return 0;
}

static PyObject *
Device_getversion(Device *self, void *closure)
{
    PyObject *version = PyLong_FromLong(self->hw->version);
    if (version == NULL) {
        return NULL;
    }

    return version;
}

static int
Device_setversion(Device *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->hw->version = PyLong_AsLong(value);

    return 0;
}

static PyObject *
Device_getname(Device *self, void *closure)
{
    PyObject *name = PyUnicode_FromString(self->hw->name.c_str());
    if (name == NULL) {
        return NULL;
    }

    return name;
}

static int
Device_setname(Device *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyUnicode_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be a string");
        return -1;
    }

    PyObject * str = PyUnicode_AsASCIIString(value);
    if (str == NULL)
        return NULL;

    char * name = PyBytes_AsString(str);
    if (name == NULL)
        return NULL;

    self->hw->name = name;
    return 0;
}

static PyGetSetDef Device_getseters[] = {
    {"id",
     (getter)Device_getid, (setter)Device_setid,
     "the id number of the device",
     NULL},
    {"manufacturer",
     (getter)Device_getmanufacturer, (setter)Device_setmanufacturer,
     "the manufacturer of the device",
     NULL},
    {"version",
     (getter)Device_getversion, (setter)Device_setversion,
     "the version number of the device",
     NULL},
    {"name",
     (getter)Device_getname, (setter)Device_setname,
     "the name of the device",
     NULL},
    {NULL}  /* Sentinel */
};

static PyObject *
Device_interrupt(Device* self)
{
    PyErr_SetString(PyExc_NotImplementedError, "Not implemented");
    return NULL;
}

static PyObject *
Device_cycle(Device* self)
{
    PyErr_SetString(PyExc_NotImplementedError, "Not implemented");
    return NULL;
}

static PyMethodDef Device_methods[] = {
    {"interrupt", (PyCFunction)Device_interrupt, METH_NOARGS,
     "Send a hardware interrupt to the device"
    },
    {"cycle", (PyCFunction)Device_cycle, METH_NOARGS,
     "Triggered before each cycle of the DCPU"
    },
    {NULL} /* Sentinel */
};

static PyTypeObject DeviceType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "saturn.device",           /* tp_name */
    sizeof(Device),            /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Device_dealloc,/* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "hardware devices",        /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Device_methods,            /* tp_methods */
    0,                         /* tp_members */
    Device_getseters,          /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Device_init,     /* tp_init */
    0,                         /* tp_alloc */
    Device_new,                /* tp_new */
};

struct DCPU {
    PyObject_HEAD

    /// the cpu the object is wrapping
    galaxy::saturn::dcpu* cpu;
};

static void
DCPU_dealloc(DCPU* self)
{
    delete self->cpu;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
DCPU_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    DCPU *self;

    self = (DCPU *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cpu = new galaxy::saturn::dcpu();
    }

    return (PyObject *)self;
}

static int
DCPU_init(DCPU *self, PyObject *args, PyObject *kwds)
{
    // nothing to do?
    return 0;
}

static PyObject *
DCPU_getA(DCPU *self, void *closure)
{
    PyObject *A = PyLong_FromLong(self->cpu->A);
    if (A == NULL) {
        return NULL;
    }

    return A;
}

static int
DCPU_setA(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->A = PyLong_AsLong(value);

    return 0;
}

static PyObject *
DCPU_getB(DCPU *self, void *closure)
{
    PyObject *B = PyLong_FromLong(self->cpu->B);
    if (B == NULL) {
        return NULL;
    }

    return B;
}

static int
DCPU_setB(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->B = PyLong_AsLong(value);

    return 0;
}

static PyObject *
DCPU_getC(DCPU *self, void *closure)
{
    PyObject *C = PyLong_FromLong(self->cpu->C);
    if (C == NULL) {
        return NULL;
    }

    return C;
}

static int
DCPU_setC(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->C = PyLong_AsLong(value);

    return 0;
}

static PyObject *
DCPU_getX(DCPU *self, void *closure)
{
    PyObject *X = PyLong_FromLong(self->cpu->X);
    if (X == NULL) {
        return NULL;
    }

    return X;
}

static int
DCPU_setX(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->X = PyLong_AsLong(value);

    return 0;
}

static PyObject *
DCPU_getY(DCPU *self, void *closure)
{
    PyObject *Y = PyLong_FromLong(self->cpu->Y);
    if (Y == NULL) {
        return NULL;
    }

    return Y;
}

static int
DCPU_setY(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->Y = PyLong_AsLong(value);

    return 0;
}

static PyObject *
DCPU_getZ(DCPU *self, void *closure)
{
    PyObject *Z = PyLong_FromLong(self->cpu->Z);
    if (Z == NULL) {
        return NULL;
    }

    return Z;
}

static int
DCPU_setZ(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->Z = PyLong_AsLong(value);

    return 0;
}

static PyObject *
DCPU_getI(DCPU *self, void *closure)
{
    PyObject *I = PyLong_FromLong(self->cpu->I);
    if (I == NULL) {
        return NULL;
    }

    return I;
}

static int
DCPU_setI(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->I = PyLong_AsLong(value);

    return 0;
}

static PyObject *
DCPU_getJ(DCPU *self, void *closure)
{
    PyObject *J = PyLong_FromLong(self->cpu->J);
    if (J == NULL) {
        return NULL;
    }

    return J;
}

static int
DCPU_setJ(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->J = PyLong_AsLong(value);

    return 0;
}

static PyObject *
DCPU_getPC(DCPU *self, void *closure)
{
    PyObject *PC = PyLong_FromLong(self->cpu->PC);
    if (PC == NULL) {
        return NULL;
    }

    return PC;
}

static int
DCPU_setPC(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->PC = PyLong_AsLong(value);

    return 0;
}

static PyObject *
DCPU_getSP(DCPU *self, void *closure)
{
    PyObject *SP = PyLong_FromLong(self->cpu->SP);
    if (SP == NULL) {
        return NULL;
    }

    return SP;
}

static int
DCPU_setSP(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->SP = PyLong_AsLong(value);

    return 0;
}

static PyObject *
DCPU_getEX(DCPU *self, void *closure)
{
    PyObject *EX = PyLong_FromLong(self->cpu->EX);
    if (EX == NULL) {
        return NULL;
    }

    return EX;
}

static int
DCPU_setEX(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->EX = PyLong_AsLong(value);

    return 0;
}

static PyObject *
DCPU_getIA(DCPU *self, void *closure)
{
    PyObject *IA = PyLong_FromLong(self->cpu->IA);
    if (IA == NULL) {
        return NULL;
    }

    return IA;
}

static int
DCPU_setIA(DCPU *self, PyObject *value, void *closure)
{
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the first attribute");
        return -1;
    }

    if (! PyLong_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
                        "The first attribute value must be an integer");
        return -1;
    }

    self->cpu->IA = PyLong_AsLong(value);

    return 0;
}

static PyGetSetDef DCPU_getseters[] = {
    {"A",
     (getter)DCPU_getA, (setter)DCPU_setA,
     "register A",
     NULL},
    {"B",
     (getter)DCPU_getB, (setter)DCPU_setB,
     "register B",
     NULL},
    {"C",
     (getter)DCPU_getC, (setter)DCPU_setC,
     "register C",
     NULL},
    {"X",
     (getter)DCPU_getX, (setter)DCPU_setX,
     "register X",
     NULL},
    {"Y",
     (getter)DCPU_getY, (setter)DCPU_setY,
     "register Y",
     NULL},
    {"Z",
     (getter)DCPU_getZ, (setter)DCPU_setZ,
     "register Z",
     NULL},
    {"I",
     (getter)DCPU_getI, (setter)DCPU_setI,
     "register I",
     NULL},
    {"J",
     (getter)DCPU_getJ, (setter)DCPU_setJ,
     "register J",
     NULL},
    {"PC",
     (getter)DCPU_getPC, (setter)DCPU_setPC,
     "register PC",
     NULL},
    {"SP",
     (getter)DCPU_getSP, (setter)DCPU_setSP,
     "register SP",
     NULL},
    {"EX",
     (getter)DCPU_getEX, (setter)DCPU_setEX,
     "register EX",
     NULL},
    {"IA",
     (getter)DCPU_getIA, (setter)DCPU_setIA,
     "register IA",
     NULL},
    {NULL}  /* Sentinel */
};

static PyObject *
DCPU_cycle(DCPU* self)
{
    try {
        self->cpu->cycle();
    } catch (galaxy::saturn::invalid_opcode& e) {
        PyErr_SetString(InvalidOpcodeError, e.what());
        return NULL;
    } catch (galaxy::saturn::queue_overflow& e) {
        PyErr_SetString(QueueOverflowError, e.what());
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
DCPU_interrupt(DCPU* self, PyObject *args)
{
    std::uint16_t msg;

    // h = short int (checks for overflow)
    if (!PyArg_ParseTuple(args, "H", &msg))
        return NULL;

    try {
        self->cpu->interrupt(msg);
    } catch (galaxy::saturn::queue_overflow& e) {
        PyErr_SetString(QueueOverflowError, e.what());
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
DCPU_attach_device(DCPU* self, PyObject *args)
{
    PyObject * dev = PyTuple_GetItem(args, 0);

    if (dev == NULL) {
        return NULL;
    }

    if (Py_TYPE(dev) != &DeviceType && !PyType_IsSubtype(Py_TYPE(dev), &DeviceType)) {
        PyErr_SetString(PyExc_TypeError, "Argument must be an instance of Device or an instance of a subclass of Device");
        return NULL;
    }

    Device* hw = (Device *) dev;
    self->cpu->attach_device(hw->hw);

    Py_RETURN_NONE;
}


static PyObject *
DCPU_flash(DCPU* self, PyObject *args)
{
    PyObject * words = PyTuple_GetItem(args, 0);

    if (words == NULL) {
        return NULL;
    }

    if (PySequence_Check(words) != 1) {
        PyErr_SetString(PyExc_TypeError, "Non-sequence argument");
        return NULL;
    }

    std::vector<std::uint16_t> mem;

    int length = PySequence_Size(words);
    if (length < 0) {
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        PyObject * word = PySequence_GetItem(words, i);
        if (word == NULL) {
            return NULL;
        }

        if (PyLong_Check(word) != 1) {
            Py_DECREF(word);
            PyErr_SetString(PyExc_TypeError, "Non-integer value in sequence");
            return NULL;
        }

        mem.push_back(PyLong_AsLong(word));
        Py_DECREF(word);
    }

    self->cpu->flash(mem.begin(), mem.end());

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
DCPU_reset(DCPU* self)
{
    self->cpu->reset();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef DCPU_methods[] = {
    {"cycle", (PyCFunction)DCPU_cycle, METH_NOARGS,
     "Run the cpu for a single cycle"
    },
    {"interrupt", (PyCFunction)DCPU_interrupt, METH_VARARGS,
     "Trigger an interrupt on the DCPU"
    },
    {"attach_device", (PyCFunction)DCPU_attach_device, METH_VARARGS,
     "Attach a device to the DCPU"
    },
    {"flash", (PyCFunction)DCPU_flash, METH_VARARGS,
     "Flash the DCPU's memory with a sequence of integers"
    },
    {"reset", (PyCFunction)DCPU_reset, METH_NOARGS,
     "Reset the DCPU's memory and registers"
    },
    {NULL} /* Sentinel */
};

static Py_ssize_t
DCPU_length (DCPU *self)
{
    return self->cpu->ram.size();
}

static PyObject *
DCPU_item (DCPU *self, Py_ssize_t i)
{
    if (i < 0 || i >= self->cpu->ram.size()) {
        PyErr_SetString(PyExc_IndexError, "RAM index out of range");
        return NULL;
    }

    PyObject *val = PyLong_FromLong(self->cpu->ram[i]);
    if (val == NULL)
        return NULL;

    return val;
}

static int
DCPU_ass_item (DCPU *self, Py_ssize_t i, PyObject *val)
{
    if (i < 0 || i >= self->cpu->ram.size()) {
        PyErr_SetString(PyExc_IndexError, "RAM assignment index out of range");
        return -1;
    }

    if (!PyLong_Check(val)) {
        return -1;
    }

    std::uint16_t word = PyLong_AsLong(val);
    self->cpu->ram[i] = word;

    return 0;
}

static PySequenceMethods DCPU_as_sequence = {
    (lenfunc)DCPU_length,                       /* sq_length */
    0,                                          /* sq_concat */
    0,                                          /* sq_repeat */
    (ssizeargfunc)DCPU_item,                    /* sq_item */
    0,                                          /* sq_slice */
    (ssizeobjargproc)DCPU_ass_item,             /* sq_ass_item */
    0,                                          /* sq_ass_slice */
    0,                                          /* sq_contains */
    0,                                          /* sq_inplace_concat */
    0,                                          /* sq_inplace_repeat */
};

static PyTypeObject DCPUType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "saturn.dcpu",             /* tp_name */
    sizeof(DCPU),              /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)DCPU_dealloc,  /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    &DCPU_as_sequence,         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "dcpu objects",            /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    DCPU_methods,              /* tp_methods */
    0,                         /* tp_members */
    DCPU_getseters,            /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)DCPU_init,       /* tp_init */
    0,                         /* tp_alloc */
    DCPU_new,                  /* tp_new */
};

static PyModuleDef saturnmodule = {
    PyModuleDef_HEAD_INIT,
    "saturn",
    "wrapper for galaxy's emulator",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_saturn(void)
{
    PyObject* m;

    if (PyType_Ready(&DCPUType) < 0)
        return NULL;

    if (PyType_Ready(&DeviceType) < 0)
        return NULL;

    m = PyModule_Create(&saturnmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&DCPUType);
    if (PyModule_AddObject(m, "dcpu", (PyObject *)&DCPUType) < 0) {
        return NULL;
    }

    Py_INCREF(&DeviceType);
    if (PyModule_AddObject(m, "device", (PyObject *)&DeviceType) < 0) {
        return NULL;
    }

    InvalidOpcodeError = PyErr_NewException("saturn.InvalidOpcodeError", NULL, NULL);
    if (InvalidOpcodeError == NULL)
        return NULL;

    Py_INCREF(InvalidOpcodeError);
    if (PyModule_AddObject(m, "InvalidOpcodeError", InvalidOpcodeError) < 0) {
        return NULL;
    }

    return m;
}
