from cccorelib import CCVector2, CCVector3

import pytest


class TestCCVector2:
    def test_gettitem(self):
        v = CCVector2(0.0, 1.0)
        assert v[0] == 0.0
        assert v[1] == 1.0

        with pytest.raises(IndexError):
            _ = v[2]

        with pytest.raises(TypeError):
            _ = v[-1]

    def test_div(self):
        v = CCVector2(1.5, 2.0)

        v_half = v / 2
        assert v_half.x == 0.75
        assert v_half.y == 1.0

        assert v_half[0] == 0.75
        assert v_half[1] == 1.0

        with pytest.raises(RuntimeError):
            _ = v / 0

        v /= 2
        assert v.x == 0.75
        assert v.y == 1.0

        assert v[0] == 0.75
        assert v[1] == 1.0

        with pytest.raises(RuntimeError):
            v /= 0

    def test_mul(self):
        v = CCVector2(1.5, 2.0)

        v2 = v * 2
        assert v2.x == 3.0
        assert v2.y == 4.0

        v *= 2
        assert v.x == 3.0
        assert v.y == 4.0


class TestCCVector3:
    def test_getitem(self):
        v = CCVector3(0.0, 1.0, 2.0)

        assert v[0] == 0.0
        assert v[1] == 1.0
        assert v[2] == 2.0

        with pytest.raises(IndexError):
            _ = v[3]

        with pytest.raises(TypeError):
            _ = v[-1]

    def test_mul(self):
        v = CCVector3(1.5, 2.0, 3.0)

        v2 = v * 2
        assert v2.x == 3.0
        assert v2.y == 4.0
        assert v2.z == 6.0

        v *= 2
        assert v.x == 3.0
        assert v.y == 4.0
        assert v.z == 6.0

    def test_div(self):
        v = CCVector3(1.5, 2.0, 3.0)

        v_half = v / 2
        assert v_half.x == 0.75
        assert v_half.y == 1.0
        assert v_half.z == 1.5

        with pytest.raises(RuntimeError):
            _ = v / 0

        v /= 2
        assert v.x == 0.75
        assert v.y == 1.0
        assert v.z == 1.5

        with pytest.raises(RuntimeError):
            v /= 0

    def test_add_sub(self):
        v = CCVector3(1.5, 2.0, 3.0)
        v2 = CCVector3(1, 2.5, 0.5)

        v3 = v + v2
        assert v3.x == 2.5
        assert v3.y == 4.5
        assert v3.z == 3.5

        v3 -= v2
        v3 += v2

        assert v3.x == 2.5
        assert v3.y == 4.5
        assert v3.z == 3.5
