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

#include "object_file.hpp"

extern "C"
{
    static PyObject * jupiter_assemble(PyObject *self, PyObject *args);
//    static PyObject * jupiter_disassemble(PyObject *self, PyObject *args);
}

static PyObject *JupiterError;


static PyObject * jupiter_assemble(PyObject *self, PyObject *args)
{
    const char *assembly_code;
    galaxy::jupiter::object_file cpp_object;

    if (!PyArg_ParseTuple(args, "s", &assembly_code))
        return NULL;
    
    try {
        std::string asm_code = assembly_code;

	cpp_object = galaxy::jupiter::assemble(asm_code.begin(),
	                                       asm_code.end());
      // change this to galaxy::exception when it is added to libjupiter
    } catch (std::exception& e) {
        PyErr_SetString(JupiterError, "Assembly failed");
        return NULL;
    }

    // Create an object_file to store results in
    object_file *obj_file = (object_file*)PyObject_CallObject((PyObject *)&object_file_type, NULL);

    if(obj_file == NULL) {
        return NULL;
    }

    // convert std::unordered_map to Python dict

    PyObject * e_labels = obj_file->exported_labels;

    for(auto it = cpp_object.exported_labels.begin(); it != cpp_object.exported_labels.end(); ++it) {
        if(PyDict_SetItem(e_labels, PyUnicode_FromString(it->first.c_str()), PyLong_FromLong(it->second)) < 0) {
            Py_DECREF(obj_file);
            return NULL;
        }
    }

    // convert std::unordered_set to Python set

    PyObject * u_labels = obj_file->used_labels;

    for(auto it = cpp_object.used_labels.begin(); it != cpp_object.used_labels.end(); ++it) {
        if(PySet_Add(u_labels, PyLong_FromLong(*it)) < 0) {
            Py_DECREF(obj_file);
            return NULL;
        }
    }

    // convert std::unordered_map to Python dict

    PyObject * i_labels = obj_file->imported_labels;

    for(auto it = cpp_object.imported_labels.begin(); it != cpp_object.imported_labels.end(); ++it) {
        if(PyDict_SetItem(i_labels, PyLong_FromLong(it->first), PyUnicode_FromString(it->second.c_str())) < 0) {
            Py_DECREF(obj_file);
            return NULL;
        }
    }

    // convert std::vector to Python list

    int length = cpp_object.object_code.size();

    // I'm fairly sure there's no way for this to be NULL
    PyObject * bytes = obj_file->object_code;

    for(int i = 0; i < length; i++) {
        if(PyList_SetItem(bytes, i, PyLong_FromLong(cpp_object.object_code[i])) < 0) {
            Py_DECREF(bytes);
            return NULL;
        }
    }

    return (PyObject *)obj_file;
}

/*
static PyObject * jupiter_disassemble(PyObject *self, PyObject *args)
{
    PyObject * bytes = PyTuple_GetItem(args, 0);

    if(bytes == NULL) {
        return NULL;
    }

    if(PySequence_Check(bytes) != 1) {
        PyErr_SetString(PyExc_TypeError, "Non-sequence argument");
        return NULL;
    }

    std::vector<std::uint16_t> object_code;
    
    int length = PySequence_Size(bytes);
    if(length < 0) {
        return NULL;
    }

    for(int i = 0; i < length; i++) {
        PyObject * byte = PySequence_GetItem(bytes, i);
        if(byte == NULL) {
            return NULL;
        }
        
        if(PyNumber_Check(byte) != 1) {
            Py_DECREF(byte);
            PyErr_SetString(PyExc_TypeError, "Non-numeric value in sequence");
            return NULL;
        }
        
        object_code.push_back(PyLong_AsLong(byte));
	Py_DECREF(byte);
    }
    
    std::vector<std::string> lines;
    lines = galaxy::jupiter::disassemble(object_code.begin(),
                                         object_code.end());
    
    length = lines.size();

    PyObject * lines_list = PyList_New(length);
    if(lines_list == NULL) {
        return NULL;
    }

    for(int i = 0; i < length; i++) {
        PyObject * line = PyUnicode_FromString(lines[i].c_str());
        if(line == NULL) {
            Py_DECREF(lines_list);
            return NULL;
        }

        if(PyList_SetItem(lines_list, i, line) < 0) {
            Py_DECREF(lines_list);
            Py_DECREF(line);
            return NULL;
        }
    }

    return lines_list;
}
*/

static PyMethodDef JupiterMethods[] = {
    {"assemble", jupiter_assemble, METH_VARARGS,
     "Assemble the given code into DCPU-16 machine language."},
/*    {"disassemble", jupiter_disassemble, METH_VARARGS,
     "Disassemble the given machine code into assembly language."}, */
    {NULL, NULL, 0, NULL}        // Sentinel
};

static struct PyModuleDef jupitermodule = {
    PyModuleDef_HEAD_INIT,
    "jupiter",    // name of module
    "Module that provides an interface with the jupiter assembler",
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

    if (PyType_Ready(&object_file_type) < 0)
        return NULL;

    Py_INCREF(&object_file_type);
    PyModule_AddObject(m, "object_file", (PyObject *)&object_file_type);

    return m;
}

