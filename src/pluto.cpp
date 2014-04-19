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

#include <cstdint>

#include <Python.h>
#include <vector>

#include <libpluto.hpp>
#include "pluto.hpp"
#include "libasteroid.hpp"
#include "asteroid.hpp"

extern "C"
{
    static PyObject * pluto_link(PyObject *self, PyObject *args);
}

static PyObject *PlutoError;

static PyObject * pluto_link(PyObject *self, PyObject *args)
{
    PyObject* listObj;

    if (!PyArg_ParseTuple(args, "O", &listObj))
        return NULL;

    std::vector<galaxy::asteroid>* asteroids = cppasteroids_from_listobj(listObj);
    if (asteroids == NULL) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PlutoError, "Bad asteroids provided");
        }
        return NULL;
    }

    if (asteroids->size() == 0) {
        PyErr_SetString(PlutoError, "You must provide objects to link");
        return NULL;
    }

    std::vector<std::uint16_t> binary;
    try {
        binary = galaxy::pluto::link(*asteroids);
        // change this to galaxy::exception when it is added to libpluto
    } catch (std::exception& e) {
        PyErr_SetString(PlutoError, e.what());
        return NULL;
    }

    PyObject * binary_as_list = PyList_New(0);
    if(binary_as_list == NULL) return NULL;

    for (auto it : binary) {
        PyObject* val = PyLong_FromLongLong(it);
        if (val == NULL) return NULL;

        PyList_Append(binary_as_list, val);
    }

    // Create an asteroid to store results in
    // no NULL check is required here because we're returning anyways
    return binary_as_list;
}


std::vector<galaxy::asteroid> * cppasteroids_from_listobj(PyObject* listObj) {
    std::vector<galaxy::asteroid> *asteroids = new std::vector<galaxy::asteroid>();

    PyObject* iterator = PyObject_GetIter(listObj);
    if (iterator == NULL) return NULL;

    if(!PyIter_Check(iterator)) {
        Py_DECREF(iterator);
        PyErr_SetString(PyExc_TypeError, "Non-iterable argument");
        return NULL;
    }

    PyObject * pyasteroid;
    while ((pyasteroid = PyIter_Next(iterator))) {
        galaxy::asteroid *single_asteroid = cppasteroid_from_pyasteroid(pyasteroid);
        if (single_asteroid == NULL) {
            Py_DECREF(pyasteroid);
            break;
        }

        asteroids->push_back(*single_asteroid);
    	Py_DECREF(pyasteroid);
    }

    Py_DECREF(iterator);
    if (PyErr_Occurred()) return NULL;

    return asteroids;
}

#define GETATTR(attr) \
    PyObject_GetAttrString(\
        pyasteroid,\
        attr\
    );\
    if (attribute == NULL) return NULL;


galaxy::asteroid * cppasteroid_from_pyasteroid(PyObject *pyasteroid)
{
    galaxy::asteroid *cpp_object = new galaxy::asteroid();

    // we should be able to collapse this in theory.
    // does not work in practice
    PyObject * key;
    PyObject * value;
    PyObject * item;
    PyObject * iterator;
    PyObject * attribute;
    Py_ssize_t pos = 0;

    attribute = GETATTR("exported_labels");
    while (PyDict_Next(attribute, &pos, &key, &value)) {
        PyObject* label_name_obj = PyUnicode_AsASCIIString(key);
        if (label_name_obj == NULL) return NULL;

        char* label_name = PyBytes_AsString(label_name_obj);
        if (label_name == NULL) return NULL;

        Py_DECREF(label_name_obj);

        cpp_object->exported_labels.emplace(
            label_name,
            PyLong_AsLong(value)
        );

        Py_DECREF(key);
        Py_DECREF(value);
    }

    Py_DECREF(attribute);
    if (PyErr_Occurred()) return NULL;

    attribute = GETATTR("used_labels");

    iterator = PyObject_GetIter(attribute);
    while ((item = PyIter_Next(iterator))) {
        cpp_object->used_labels.insert(
            PyLong_AsLong(item)
        );
        Py_DECREF(item);
    }
    Py_DECREF(iterator);

    Py_DECREF(attribute);
    if (PyErr_Occurred()) return NULL;

    // convert std::unordered_map to Python dict
    attribute = GETATTR("imported_labels");
    pos = 0;
    while (PyDict_Next(attribute, &pos, &key, &value)) {
        PyObject* label_name_obj = PyUnicode_AsASCIIString(value);
        if (label_name_obj == NULL) return NULL;

        char* label_name = PyBytes_AsString(label_name_obj);
        if (label_name == NULL) return NULL;

        Py_DECREF(label_name_obj);

        cpp_object->imported_labels.emplace(
            PyLong_AsLong(key),
            label_name
        );

        Py_DECREF(key);
        Py_DECREF(value);
    }

    Py_DECREF(attribute);
    if (PyErr_Occurred()) return NULL;

    // convert std::vector to Python list
    attribute = GETATTR("object_code")

    iterator = PyObject_GetIter(attribute);
    while ((item = PyIter_Next(iterator))) {
        cpp_object->object_code.insert(
            cpp_object->object_code.begin(),
            1,
            PyLong_AsLong(item)
        );
        Py_DECREF(item);
    }
    Py_DECREF(iterator);

    Py_DECREF(attribute);
    if (PyErr_Occurred()) return NULL;

    return cpp_object;
}


static PyMethodDef PlutoMethods[] = {
    {"link", pluto_link, METH_VARARGS,
     "Links the given asteroid objects into DCPU-16 machine language."},
    {NULL, NULL, 0, NULL}        // Sentinel
};

static struct PyModuleDef plutomodule = {
    PyModuleDef_HEAD_INIT,
    "pluto",    // name of module
    "Module that provides an interface with the galaxy linker",
    -1,           // module keeps state in global variables
    PlutoMethods
};

PyMODINIT_FUNC
PyInit_pluto(void)
{
    PyObject *m;

    m = PyModule_Create(&plutomodule);
    if (m == NULL)
        return NULL;

    PlutoError = PyErr_NewException("pluto.error", NULL, NULL);
    if (PlutoError == NULL)
        return NULL;

    Py_INCREF(PlutoError);
    if (PyModule_AddObject(m, "error", PlutoError) < 0) {
        return NULL;
    }

    if (PyType_Ready(&asteroid_type) < 0){
        Py_DECREF(PlutoError);
        return NULL;
    }

    return m;
}

