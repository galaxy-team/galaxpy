#include "Python.h"
#include <vector>

PyObject* pyasteroid_from_cppasteroid(galaxy::asteroid cpp_object);
std::vector<galaxy::asteroid> * cppasteroids_from_listobj(PyObject* listObj);
galaxy::asteroid * cppasteroid_from_pyasteroid(PyObject *pyasteroid);
