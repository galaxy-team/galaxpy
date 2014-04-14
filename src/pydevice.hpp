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

#ifndef PYDEVICE_HPP
#define PYDEVICE_HPP

#include <Python.h>
#include <libsaturn.hpp>
#include <cstdint>

/**
 * an interface for python devices
 */
class PyDevice : public galaxy::saturn::device {
    protected:
        PyObject & dev;

    public:
        /// the PyDevice will wrap the dev python object
        PyDevice(PyObject & dev) : galaxy::saturn::device(0,0,0,""), dev(dev) {}

        virtual void interrupt();
        virtual void cycle();
};

#endif
