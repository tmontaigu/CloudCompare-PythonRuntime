#include <pybind11/pybind11.h>

#include <SquareMatrix.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_SquareMatrix(py::module &m)
{
    py::class_<CCCoreLib::SquareMatrix>(m, "SquareMatrix")
        .def(py::init<>())
        .def("size", &CCCoreLib::SquareMatrix::size, R"doc(Returns the size of the matrix)doc")
        .def("isValid", &CCCoreLib::SquareMatrix::isValid, R"doc(Returns true if the matrix is valid)doc")
        .def("invalidate", &CCCoreLib::SquareMatrix::invalidate, R"doc(Invalidates the matrix)doc")
        .def("setValue",
             &CCCoreLib::SquareMatrix::setValue,
             R"doc(Sets a value in the matrix)doc",
             "row"_a,
             "column"_a,
             "value"_a)
        .def("getValue",
             &CCCoreLib::SquareMatrix::getValue,
             R"doc(Returns a value from the matrix)doc",
             "row"_a,
             "column"_a)
        .def("__add__", &CCCoreLib::SquareMatrix::operator+, R"doc(Addition operator)doc", "other"_a)
        .def("__sub__", &CCCoreLib::SquareMatrix::operator-, R"doc(Subtraction operator)doc", "other"_a)
        .def("__iadd__",
             &CCCoreLib::SquareMatrix::operator+=,
             R"doc(In-place addition operator)doc",
             "other"_a)
        .def("__isub__",
             &CCCoreLib::SquareMatrix::operator-=,
             R"doc(In-place subtraction operator)doc",
             "other"_a);
}