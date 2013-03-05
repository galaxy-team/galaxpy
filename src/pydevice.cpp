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
#include <libsaturn.hpp>
#include <device.hpp>
#include "pydevice.hpp"

void PyDevice::interrupt()
{
    PyObject * pfunc = PyObject_GetAttrString(dev, "interrupt");
    if (pfunc && PyCallable_Check(pfunc))
        PyObject_CallObject(pfunc, NULL);

    Py_XDECREF(pfunc);
}

void PyDevice::cycle()
{
    PyObject * pfunc = PyObject_GetAttrString(dev, "cycle");
    if (pfunc && PyCallable_Check(pfunc))
        PyObject_CallObject(pfunc, NULL);

    Py_XDECREF(pfunc);
}
