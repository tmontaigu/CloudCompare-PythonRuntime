// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonRuntime                       #
// #                                                                        #
// #  This program is free software; you can redistribute it and/or modify  #
// #  it under the terms of the GNU General Public License as published by  #
// #  the Free Software Foundation; version 2 of the License.               #
// #                                                                        #
// #  This program is distributed in the hope that it will be useful,       #
// #  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
// #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
// #  GNU General Public License for more details.                          #
// #                                                                        #
// #                   COPYRIGHT: Thomas Montaigu                           #
// #                                                                        #
// ##########################################################################

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <Jacobi.h>
#include <SquareMatrix.h>

namespace py = pybind11;
using namespace pybind11::literals;

void define_Jacobi(py::module &cccorelib)
{
    // Only the double instantiation of Jacobi makes sense
    // because it's the only one used in CC and python's float are double
    using Jacobi_d = CCCoreLib::Jacobi<double>;
    using SquareMatrixd = CCCoreLib::SquareMatrixd;

    py::class_<Jacobi_d> PyJacobi(cccorelib, "Jacobi", R"doc(
Jacobi eigenvalue / eigenvector decomposition for symmetric SquareMatrixd.
)doc");

    PyJacobi
        .def_static(
            "ComputeEigenValuesAndVectors",
            [](SquareMatrixd &matrix, bool absoluteValues, unsigned maxIterationCount)
            {
                SquareMatrixd eigenVectors;
                std::vector<double> eigenValues;
                if (!Jacobi_d::ComputeEigenValuesAndVectors(
                        matrix, eigenVectors, eigenValues, absoluteValues, maxIterationCount))
                {
                    throw std::runtime_error("Jacobi.ComputeEigenValuesAndVectors failed (invalid matrix, "
                                             "allocation failure, or no convergence within "
                                             "maxIterationCount sweeps).");
                }
                return py::make_tuple(eigenVectors, eigenValues);
            },
            "matrix"_a,
            "absoluteValues"_a = true,
            "maxIterationCount"_a = 50,
            R"doc(
Compute eigenvalues and eigenvectors of a symmetric SquareMatrixd.

Parameters
----------
matrix : SquareMatrixd
    Symmetric square matrix (modified in place).
absoluteValues : bool, optional
    If True (default), the eigenvalues are returned as absolute values.
maxIterationCount : int, optional
    Maximum number of Jacobi sweeps before giving up. Default 50.

Returns
-------
tuple (eigenVectors, eigenValues)
    ``eigenVectors`` is a SquareMatrixd whose columns are the
    eigenvectors; ``eigenValues`` is a list of floats in the same order
    as the columns.

Raises
------
RuntimeError
    If the matrix is invalid, allocation fails, or the algorithm does
    not converge within ``maxIterationCount`` sweeps.
)doc")

        .def_static(
            "SortEigenValuesAndVectors",
            // Take by value so the original Python objects are untouched; we
            // return freshly sorted copies. This is a more natural Python
            // contract than the C++ in-place mutation.
            [](SquareMatrixd eigenVectors, std::vector<double> eigenValues)
            {
                if (!Jacobi_d::SortEigenValuesAndVectors(eigenVectors, eigenValues))
                {
                    throw std::runtime_error(
                        "Jacobi.SortEigenValuesAndVectors failed (invalid matrix, "
                        "size < 2, or size mismatch between eigenVectors and eigenValues).");
                }
                return py::make_tuple(eigenVectors, eigenValues);
            },
            "eigenVectors"_a,
            "eigenValues"_a,
            R"doc(
Return a copy of the eigen pair sorted by decreasing eigenvalue.

Unlike the C++ version (which sorts in place), the Python binding
returns new ``(sortedVectors, sortedValues)`` objects so mutation
semantics are explicit.
)doc")

        .def_static(
            "GetEigenVector",
            [](const SquareMatrixd &eigenVectors, unsigned index)
            {
                std::vector<double> out(eigenVectors.size());
                if (!Jacobi_d::GetEigenVector(eigenVectors, index, out.data()))
                {
                    throw std::runtime_error("Jacobi.GetEigenVector: index out of bounds.");
                }
                return out;
            },
            "eigenVectors"_a,
            "index"_a,
            "Return the ``index``-th eigenvector (column of ``eigenVectors``) as a list of floats.")

        .def_static(
            "GetMaxEigenValueAndVector",
            [](const SquareMatrixd &eigenVectors, const std::vector<double> &eigenValues)
            {
                double maxValue = 0.0;
                std::vector<double> maxVector(eigenVectors.size());
                if (!Jacobi_d::GetMaxEigenValueAndVector(
                        eigenVectors, eigenValues, maxValue, maxVector.data()))
                {
                    throw std::runtime_error("Jacobi.GetMaxEigenValueAndVector failed (invalid matrix, "
                                             "size < 2, or size mismatch).");
                }
                return py::make_tuple(maxValue, maxVector);
            },
            "eigenVectors"_a,
            "eigenValues"_a,
            "Return ``(maxEigenvalue, maxEigenvector_as_list)``.")

        .def_static(
            "GetMinEigenValueAndVector",
            [](const SquareMatrixd &eigenVectors, const std::vector<double> &eigenValues)
            {
                double minValue = 0.0;
                std::vector<double> minVector(eigenVectors.size());
                if (!Jacobi_d::GetMinEigenValueAndVector(
                        eigenVectors, eigenValues, minValue, minVector.data()))
                {
                    throw std::runtime_error("Jacobi.GetMinEigenValueAndVector failed (invalid matrix, "
                                             "size < 2, or size mismatch).");
                }
                return py::make_tuple(minValue, minVector);
            },
            "eigenVectors"_a,
            "eigenValues"_a,
            "Return ``(minEigenvalue, minEigenvector_as_list)``.")

        .def_static("TestEigenVecAndValues",
                    &Jacobi_d::TestEigenVecAndValues,
                    "matrix"_a,
                    "eigenVectors"_a,
                    "eigenValues"_a,
                    R"doc(
Check consistency of a ``(matrix, eigenVectors, eigenValues)`` triple.

Returns True when ``matrix @ v ≈ lambda * v`` for each column of
``eigenVectors`` (within 1e-5), False otherwise.

.. note::
    The underlying C++ implementation assumes a 3x3 matrix — use this
    only for 3x3 cases.
)doc");
}
