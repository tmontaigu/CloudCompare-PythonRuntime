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


def test_as_array():
    scalar_field = cccorelib.ScalarField("Codification")
    scalar_field.resize(10)

    array = scalar_field.asArray()

    # Changes made to the scalar_field reflects on the array
    scalar_field.fill(1.0)
    assert np.all(array == 1.0)

    # and vice-versa
    array[:] = 2.0
    assert scalar_field[0] == 2.0
    assert np.all(array == 2.0)
