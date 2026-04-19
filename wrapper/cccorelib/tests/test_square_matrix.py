from cccorelib import SquareMatrix, SquareMatrixd

import pytest


# Both types share the same API (SquareMatrixTpl<PointCoordinateType> vs
# SquareMatrixTpl<double>), so parametrize the same checks over both.
@pytest.fixture(params=[SquareMatrix, SquareMatrixd], ids=["SquareMatrix", "SquareMatrixd"])
def MatrixType(request):
    return request.param


class TestConstruction:
    def test_default_is_invalid(self, MatrixType):
        m = MatrixType()
        assert m.size() == 0
        assert not m.isValid()

    def test_sized_is_valid(self, MatrixType):
        m = MatrixType(3)
        assert m.size() == 3
        assert m.isValid()

    def test_zero_sized_is_invalid(self, MatrixType):
        m = MatrixType(0)
        assert m.size() == 0
        assert not m.isValid()


class TestGetSetValue:
    def test_set_then_get(self, MatrixType):
        m = MatrixType(3)
        m.setValue(0, 0, 1.0)
        m.setValue(1, 2, 2.5)
        m.setValue(2, 1, -4.0)
        assert m.getValue(0, 0) == 1.0
        assert m.getValue(1, 2) == 2.5
        assert m.getValue(2, 1) == -4.0

    def test_set_overwrites(self, MatrixType):
        m = MatrixType(2)
        m.setValue(0, 0, 1.0)
        m.setValue(0, 0, 7.0)
        assert m.getValue(0, 0) == 7.0


class TestInvalidate:
    def test_invalidate_resets_size(self, MatrixType):
        m = MatrixType(4)
        assert m.isValid()
        m.invalidate()
        assert m.size() == 0
        assert not m.isValid()


def _fill(m, values):
    n = m.size()
    for i in range(n):
        for j in range(n):
            m.setValue(i, j, values[i][j])


def _to_nested(m):
    n = m.size()
    return [[m.getValue(i, j) for j in range(n)] for i in range(n)]


class TestArithmetic:
    def test_addition(self, MatrixType):
        a = MatrixType(2)
        b = MatrixType(2)
        _fill(a, [[1.0, 2.0], [3.0, 4.0]])
        _fill(b, [[10.0, 20.0], [30.0, 40.0]])

        c = a + b
        assert _to_nested(c) == [[11.0, 22.0], [33.0, 44.0]]
        # Originals unchanged
        assert _to_nested(a) == [[1.0, 2.0], [3.0, 4.0]]
        assert _to_nested(b) == [[10.0, 20.0], [30.0, 40.0]]

    def test_subtraction(self, MatrixType):
        a = MatrixType(2)
        b = MatrixType(2)
        _fill(a, [[10.0, 20.0], [30.0, 40.0]])
        _fill(b, [[1.0, 2.0], [3.0, 4.0]])

        c = a - b
        assert _to_nested(c) == [[9.0, 18.0], [27.0, 36.0]]

    def test_inplace_addition(self, MatrixType):
        a = MatrixType(2)
        b = MatrixType(2)
        _fill(a, [[1.0, 2.0], [3.0, 4.0]])
        _fill(b, [[10.0, 20.0], [30.0, 40.0]])

        a += b
        assert _to_nested(a) == [[11.0, 22.0], [33.0, 44.0]]

    def test_inplace_subtraction(self, MatrixType):
        a = MatrixType(2)
        b = MatrixType(2)
        _fill(a, [[10.0, 20.0], [30.0, 40.0]])
        _fill(b, [[1.0, 2.0], [3.0, 4.0]])

        a -= b
        assert _to_nested(a) == [[9.0, 18.0], [27.0, 36.0]]


class TestSquareMatrixdPrecision:
    """Checks specific to the double variant — values outside float precision
    must round-trip exactly."""

    def test_double_precision_preserved(self):
        m = SquareMatrixd(1)
        # A value that cannot be represented exactly as a float32
        big = 1.0 + 1e-12
        m.setValue(0, 0, big)
        assert m.getValue(0, 0) == big

    def test_square_matrix_and_squarematrixd_are_distinct_types(self):
        assert SquareMatrix is not SquareMatrixd
        assert not isinstance(SquareMatrix(1), SquareMatrixd)
        assert not isinstance(SquareMatrixd(1), SquareMatrix)
