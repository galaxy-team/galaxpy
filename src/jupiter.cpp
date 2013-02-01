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

Your copy of the GNU General Public License should be in the
file named "LICENSE-LGPL.txt".

*/

#include <Python.h>
#include <libjupiter.hpp>
#include <vector>
#include <cstdint>

extern "C"
{
    static PyObject * jupiter_assemble(PyObject *self, PyObject *args);
    // not supported yet:
    // static PyObject * jupiter_disassemble(PyObject *self, PyObject *args);
}

static PyObject *JupiterError;

static PyObject * jupiter_assemble(PyObject *self, PyObject *args)
{
    const char *assembly_code;
    std::vector<std::uint16_t> object_code;

    if (!PyArg_ParseTuple(args, "s", &assembly_code))
        return NULL;
    
    try {
        std::string asm_code = assembly_code;

	object_code = galaxy::jupiter::assemble(asm_code.begin(),
	                                        asm_code.end());
      // change this to galaxy::exception when it is added to libjupiter
    } catch (std::exception& e) {
        PyErr_SetString(JupiterError, "Assembly failed");
        return NULL;
    }

    // convert std::vector to Python list

    int length = object_code.size();

    PyObject * bytes = PyList_New(length);

    for(int i = 0; i < length; i++) {
        if(PyList_SetItem(bytes, i, PyLong_FromLong(object_code[i])) < 0) {
            Py_DECREF(bytes);
            return NULL;
        }
    }

    return bytes;
}

static PyMethodDef JupiterMethods[] = {
    {"assemble", jupiter_assemble, METH_VARARGS,
     "Assemble the given code into DCPU-16 machine language."},
    {NULL, NULL, 0, NULL}        // Sentinel
};

static struct PyModuleDef jupitermodule = {
    PyModuleDef_HEAD_INIT,
    "jupiter",    // name of module
    NULL,  // module documentation
    -1,           // module keeps state in global variables
    JupiterMethods
};

PyMODINIT_FUNC
PyInit_jupiter(void)
{
    PyObject *m;

    m = PyModule_Create(&jupitermodule);
    if (m == NULL)
        return NULL;

    JupiterError = PyErr_NewException("jupiter.error", NULL, NULL);
    Py_INCREF(JupiterError);
    PyModule_AddObject(m, "error", JupiterError);
    return m;
}

