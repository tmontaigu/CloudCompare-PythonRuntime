//##########################################################################
//#                                                                        #
//#                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                   COPYRIGHT: Thomas Montaigu                           #
//#                                                                        #
//##########################################################################

#ifndef CLOUDCOMPAREPROJECTS_WRAPPERS_H
#define CLOUDCOMPAREPROJECTS_WRAPPERS_H

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

template <class T> using observer_ptr = std::unique_ptr<T, py::nodelete>;

namespace PyCC
{
void NoOpDelete(void *) {}

template <class T> py::array_t<T> VectorAsNumpyArray(std::vector<T> &vector)
{
    // https://stackoverflow.com/questions/44659924/returning-numpy-arrays-via-pybind11
    // https://github.com/pybind/pybind11/issues/1042
    auto capsule = py::capsule(vector.data(), NoOpDelete);
    return py::array(vector.size(), vector.data(), capsule);
}
} // namespace PyCC

#endif // CLOUDCOMPAREPROJECTS_WRAPPERS_H
