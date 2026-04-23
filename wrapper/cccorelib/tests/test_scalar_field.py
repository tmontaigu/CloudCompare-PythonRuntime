import cccorelib
import pytest
import numpy as np


def test_get_set_name():
    sf = cccorelib.ScalarField("numbers")
    assert sf.getName() == "numbers"

    sf.setName("codes")
    assert sf.getName() == "codes"


def test_indexing():
    scalar_field = cccorelib.ScalarField("Codification")
    scalar_field.resize(3)

    scalar_field.setValue(1, 1.0)
    scalar_field[2] = 2.0

    assert scalar_field.getValue(0) == scalar_field[0]
    assert scalar_field.getValue(1) == scalar_field[1]
    assert scalar_field.getValue(2) == scalar_field[2]

    scalar_field.computeMinAndMax()
    assert scalar_field.getMin() == 0.0
    assert scalar_field.getMax() == 2.0

    # Try out of range indices
    with pytest.raises(IndexError):
        _ = scalar_field.getValue(4)

    with pytest.raises(IndexError):
        _ = scalar_field[4]

    with pytest.raises(IndexError):
        scalar_field.setValue(4, 0.0)

    with pytest.raises(IndexError):
        scalar_field[4] = 0.0


# def test_as_array():
#     scalar_field = cccorelib.ScalarField("Codification")
#     scalar_field.resize(10)
#
#     array = scalar_field.asArray()
#
#     # Changes made to the scalar_field reflects on the array
#     scalar_field.fill(1.0)
#     assert np.all(array == 1.0)
#
#     # and vice-versa
#     array[:] = 2.0
#     assert scalar_field[0] == 2.0
#     assert np.all(array == 2.0)


def test_clear():
    sf = cccorelib.ScalarField("sf")
    sf.resize(5, valueForNewElements=3.0)
    assert sf.size() == 5

    sf.clear()
    assert sf.size() == 0
    # After clear, the offset state is reset too; reseeding should work.
    sf.resize(3, valueForNewElements=1.0)
    assert sf.size() == 3


def test_offset_round_trip():
    sf = cccorelib.ScalarField("sf")
    sf.resize(1)
    # Small magnitudes (<100) are coerced to 0 by the C++ side.
    sf.setOffset(5000.0)
    assert sf.getOffset() == 5000.0

    sf.setOffset(0.5)
    assert sf.getOffset() == 0.0  # small-magnitude guard

    sf.setOffset(1e6)
    sf.resetOffset()
    assert sf.getOffset() == 0.0


def test_count_valid_values():
    sf = cccorelib.ScalarField("sf")
    sf.resize(5, valueForNewElements=1.0)
    assert sf.countValidValues() == 5

    sf.flagValueAsInvalid(0)
    sf.flagValueAsInvalid(2)
    assert sf.countValidValues() == 3


def test_invert_negates_values():
    sf = cccorelib.ScalarField("sf")
    sf.resize(3)
    sf.setValue(0, 1.0)
    sf.setValue(1, 2.0)
    sf.setValue(2, -3.0)

    sf.invert()

    assert sf.getValue(0) == pytest.approx(-1.0)
    assert sf.getValue(1) == pytest.approx(-2.0)
    assert sf.getValue(2) == pytest.approx(3.0)


def test_swap():
    sf = cccorelib.ScalarField("sf")
    sf.resize(3)
    sf.setValue(0, 10.0)
    sf.setValue(1, 20.0)
    sf.setValue(2, 30.0)

    sf.swap(0, 2)
    assert sf.getValue(0) == pytest.approx(30.0)
    assert sf.getValue(2) == pytest.approx(10.0)
    # Middle untouched.
    assert sf.getValue(1) == pytest.approx(20.0)


def test_local_value_accessors():
    sf = cccorelib.ScalarField("sf")
    sf.resize(3)

    # setValue applies the offset on the way in; set/getLocalValue don't.
    sf.setValue(0, 1_000_000.0)  # this also seeds the offset
    # Raw stored float is (1_000_000 - offset), which after the small-magnitude
    # rounding rule should round-trip cleanly in local-value land:
    assert sf.getLocalValue(0) + sf.getOffset() == pytest.approx(1_000_000.0)

    sf.setLocalValue(1, 7.5)
    assert sf.getLocalValue(1) == pytest.approx(7.5)


def test_get_local_values_returns_numpy_array():
    sf = cccorelib.ScalarField("sf")
    sf.resize(4)
    sf.setLocalValue(0, 0.0)
    sf.setLocalValue(1, 1.0)
    sf.setLocalValue(2, 2.0)
    sf.setLocalValue(3, 3.0)

    arr = sf.getLocalValues()
    assert isinstance(arr, np.ndarray)
    assert arr.dtype == np.float32
    assert arr.shape == (4,)
    assert arr.tolist() == [0.0, 1.0, 2.0, 3.0]
