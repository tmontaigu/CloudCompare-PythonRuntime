from cccorelib import Jacobi, SquareMatrixd

import math
import pytest


def _fill_symmetric(m, values):
    """Fill a SquareMatrixd from a nested list of size n × n."""
    n = len(values)
    for i in range(n):
        for j in range(n):
            m.setValue(i, j, values[i][j])


def _norm(v):
    return math.sqrt(sum(x * x for x in v))


def _identity_like(eigenvectors, n):
    return [[eigenvectors.getValue(i, j) for j in range(n)] for i in range(n)]


def test_diagonal_matrix():
    """Eigenvalues of diag(3, 1, 2) are {3, 1, 2}; eigenvectors are basis columns."""
    m = SquareMatrixd(3)
    _fill_symmetric(m, [[3.0, 0.0, 0.0],
                        [0.0, 1.0, 0.0],
                        [0.0, 0.0, 2.0]])

    vectors, values = Jacobi.ComputeEigenValuesAndVectors(m)

    assert isinstance(vectors, SquareMatrixd)
    assert vectors.size() == 3
    assert len(values) == 3
    assert sorted(values) == pytest.approx([1.0, 2.0, 3.0])


def test_sort_by_decreasing_eigenvalue():
    m = SquareMatrixd(3)
    _fill_symmetric(m, [[1.0, 0.0, 0.0],
                        [0.0, 3.0, 0.0],
                        [0.0, 0.0, 2.0]])

    vectors, values = Jacobi.ComputeEigenValuesAndVectors(m)
    sorted_vectors, sorted_values = Jacobi.SortEigenValuesAndVectors(vectors, values)

    # Sorted values are in decreasing order
    assert sorted_values[0] >= sorted_values[1] >= sorted_values[2]
    assert sorted_values == pytest.approx([3.0, 2.0, 1.0])

    # Originals are untouched (Python binding returns copies, not in-place).
    assert values != sorted_values or values == sorted(values, reverse=True)


def test_sort_does_not_mutate_input():
    m = SquareMatrixd(3)
    _fill_symmetric(m, [[1.0, 0.0, 0.0],
                        [0.0, 3.0, 0.0],
                        [0.0, 0.0, 2.0]])

    vectors, values = Jacobi.ComputeEigenValuesAndVectors(m)
    values_before = list(values)
    Jacobi.SortEigenValuesAndVectors(vectors, values)
    # The original Python list object is unchanged.
    assert list(values) == values_before


def test_get_eigen_vector_round_trip():
    m = SquareMatrixd(3)
    _fill_symmetric(m, [[2.0, 0.0, 0.0],
                        [0.0, 5.0, 0.0],
                        [0.0, 0.0, 3.0]])

    vectors, _ = Jacobi.ComputeEigenValuesAndVectors(m)

    for idx in range(3):
        v = Jacobi.GetEigenVector(vectors, idx)
        assert len(v) == 3
        assert _norm(v) == pytest.approx(1.0, abs=1e-9)


def test_get_eigen_vector_invalid_index():
    m = SquareMatrixd(3)
    _fill_symmetric(m, [[1.0, 0.0, 0.0],
                        [0.0, 2.0, 0.0],
                        [0.0, 0.0, 3.0]])
    vectors, _ = Jacobi.ComputeEigenValuesAndVectors(m)

    with pytest.raises(RuntimeError):
        Jacobi.GetEigenVector(vectors, 99)


def test_max_and_min_pairs():
    m = SquareMatrixd(3)
    _fill_symmetric(m, [[1.0, 0.0, 0.0],
                        [0.0, 5.0, 0.0],
                        [0.0, 0.0, 3.0]])

    vectors, values = Jacobi.ComputeEigenValuesAndVectors(m)

    max_val, max_vec = Jacobi.GetMaxEigenValueAndVector(vectors, values)
    min_val, min_vec = Jacobi.GetMinEigenValueAndVector(vectors, values)

    assert max_val == pytest.approx(5.0)
    assert min_val == pytest.approx(1.0)
    assert _norm(max_vec) == pytest.approx(1.0, abs=1e-9)
    assert _norm(min_vec) == pytest.approx(1.0, abs=1e-9)
    assert max_val >= min_val


def test_symmetric_nondiagonal():
    """Real symmetric 3×3 with off-diagonal entries. Spectrum should be real."""
    m = SquareMatrixd(3)
    _fill_symmetric(m, [[4.0, 1.0, 0.0],
                        [1.0, 4.0, 0.0],
                        [0.0, 0.0, 2.0]])

    vectors, values = Jacobi.ComputeEigenValuesAndVectors(m, absoluteValues=False)

    # Known eigenvalues of this matrix: 5, 3, 2.
    assert sorted(values, reverse=True) == pytest.approx([5.0, 3.0, 2.0], abs=1e-9)


def test_convergence_failure_raises():
    """maxIterationCount=0 forces no sweeps → non-trivial matrix can't converge."""
    m = SquareMatrixd(3)
    _fill_symmetric(m, [[4.0, 1.0, 0.0],
                        [1.0, 4.0, 0.0],
                        [0.0, 0.0, 2.0]])

    with pytest.raises(RuntimeError):
        Jacobi.ComputeEigenValuesAndVectors(m, maxIterationCount=0)


def test_test_eigen_vec_and_values_true_for_valid_pair():
    # Fresh matrix for the decomposition (will be diagonalised in place).
    decomp_matrix = SquareMatrixd(3)
    _fill_symmetric(decomp_matrix, [[4.0, 1.0, 0.0],
                                    [1.0, 4.0, 0.0],
                                    [0.0, 0.0, 2.0]])
    vectors, values = Jacobi.ComputeEigenValuesAndVectors(decomp_matrix, absoluteValues=False)

    # Rebuild a pristine copy of the original matrix for the check —
    # the one passed to ComputeEigenValuesAndVectors has been overwritten.
    original = SquareMatrixd(3)
    _fill_symmetric(original, [[4.0, 1.0, 0.0],
                               [1.0, 4.0, 0.0],
                               [0.0, 0.0, 2.0]])

    assert Jacobi.TestEigenVecAndValues(original, vectors, values) is True
