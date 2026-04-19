#include <pybind11/pybind11.h>

#include <SquareMatrix.h>

namespace py = pybind11;
using namespace pybind11::literals;

namespace
{
template <typename Scalar> void bind_square_matrix(py::module &m, const char *name)
{
    using Matrix = CCCoreLib::SquareMatrixTpl<Scalar>;
    py::class_<Matrix>(m, name)
        .def(py::init<>())
        .def(py::init<unsigned>(), "size"_a)
        .def("size", &Matrix::size, R"doc(Returns the size of the matrix)doc")
        .def("isValid", &Matrix::isValid, R"doc(Returns true if the matrix is valid)doc")
        .def("invalidate", &Matrix::invalidate, R"doc(Invalidates the matrix)doc")
        .def("setValue",
             &Matrix::setValue,
             R"doc(Sets a value in the matrix)doc",
             "row"_a,
             "column"_a,
             "value"_a)
        .def("getValue", &Matrix::getValue, R"doc(Returns a value from the matrix)doc", "row"_a, "column"_a)
        .def("__add__", &Matrix::operator+, R"doc(Addition operator)doc", "other"_a)
        .def("__sub__", &Matrix::operator-, R"doc(Subtraction operator)doc", "other"_a)
        .def("__iadd__", &Matrix::operator+=, R"doc(In-place addition operator)doc", "other"_a)
        .def("__isub__", &Matrix::operator-=, R"doc(In-place subtraction operator)doc", "other"_a);
}
} // namespace

void define_SquareMatrix(py::module &m)
{
    // SquareMatrix = SquareMatrixTpl<PointCoordinateType> (float by default).
    bind_square_matrix<PointCoordinateType>(m, "SquareMatrix");
    // SquareMatrixd = SquareMatrixTpl<double> — used e.g. by
    // Neighbourhood::computeCovarianceMatrix().
    bind_square_matrix<double>(m, "SquareMatrixd");
}
