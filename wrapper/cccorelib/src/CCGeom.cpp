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

#include <pybind11/pybind11.h>

#include <CCGeom.h>

namespace py = pybind11;
using namespace pybind11::literals;

#define DEFINE_VECTOR2TPL_TYPE(cppname, pyname, type_)                                                       \
    py::class_<cppname>(cccorelib, pyname)                                                                   \
        .def(py::init<>())                                                                                   \
        .def(py::init<type_, type_>())                                                                       \
        .def_readwrite("x", &cppname::x)                                                                     \
        .def_readwrite("y", &cppname::y)                                                                     \
        .def("norm2", &cppname::norm2)                                                                       \
        .def("norm", &cppname::norm)                                                                         \
        .def("normalize", &cppname::normalize)                                                               \
        .def("dot", &cppname::dot)                                                                           \
        .def("cross", &cppname::cross)                                                                       \
        .def("__getitem__", [](const cppname &self, unsigned index) { return self[index]; })                 \
        .def("__repr__", [](const cppname &self) {                                                           \
            return std::string("<") + pyname + "(" + std::to_string(self.x) + ", " +                         \
                   std::to_string(self.y) + ")>";                                                            \
        });

#define DEFINE_TUPLE3TPL(cppname, pyname, type_)                                                             \
    py::class_<cppname>(cccorelib, pyname)                                                                   \
        .def(py::init<>())                                                                                   \
        .def(py::init<type_, type_, type_>())                                                                \
        .def_readwrite("x", &cppname::x)                                                                     \
        .def_readwrite("y", &cppname::y)                                                                     \
        .def_readwrite("z", &cppname::z)                                                                     \
        .def("__getitem__", [](const cppname &self, unsigned index) {                                        \
            switch (index)                                                                                   \
            {                                                                                                \
            case 0:                                                                                          \
                return self.x;                                                                               \
            case 1:                                                                                          \
                return self.y;                                                                               \
            case 2:                                                                                          \
                return self.z;                                                                               \
            default:                                                                                         \
                throw std::out_of_range("index out of range");                                               \
            }                                                                                                \
        });

#define DEFINE_CCVECTOR3(cppname, pyname)                                                                    \
    py::class_<cppname>(cccorelib, pyname)                                                                   \
        .def(py::init<>())                                                                                   \
        .def(py::init<PointCoordinateType, PointCoordinateType, PointCoordinateType>())                      \
        .def_readwrite("x", &cppname::x)                                                                     \
        .def_readwrite("y", &cppname::y)                                                                     \
        .def_readwrite("z", &cppname::z)                                                                     \
        .def("__mul__", [](const cppname &self, PointCoordinateType val) { return self * val; })             \
        .def("__sub__", [](const cppname &self, const cppname &other) { return self - other; })              \
        .def("__div__", &cppname::operator/)                                                                 \
        .def("__add__", &cppname::operator+)                                                                 \
        .def("__repr__", [](const cppname &self) {                                                           \
            return std::string("<") + pyname + "(" + std::to_string(self.x) + ", " + std::to_string(self.y) + ", " +      \
                   std::to_string(self.z) + ")>";                                                            \
        });
void define_CCGeom(py::module &cccorelib)
{
    DEFINE_VECTOR2TPL_TYPE(CCVector2, "CCVector2", PointCoordinateType);
    DEFINE_VECTOR2TPL_TYPE(CCVector2d, "CCVector2d", double);
    //	DEFINE_VECTOR2TPL_TYPE(CCVector2i, "CCVector2i", int);

    DEFINE_TUPLE3TPL(Tuple3ub, "Tuple3ub", unsigned char);
    DEFINE_TUPLE3TPL(Tuple3s, "Tuple3s", short);
    DEFINE_TUPLE3TPL(Tuple3i, "Tuple3i", int);
    DEFINE_TUPLE3TPL(Tuple3ui, "Tuple3ui", unsigned int);

    DEFINE_CCVECTOR3(CCVector3, "CCVector3");
    DEFINE_CCVECTOR3(CCVector3d, "CCVector3d");
}