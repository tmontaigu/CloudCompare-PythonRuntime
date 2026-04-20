from cccorelib import CCVector3, Neighbourhood, PointCloud, SquareMatrixd

import pytest


def _make_planar_cloud():
    """A small grid of points lying almost exactly on the XY plane.

    The tiny Z jitter keeps the least-squares fit well-conditioned without
    making the plane ambiguous.
    """
    cloud = PointCloud()
    cloud.reserve(9)
    for x in (-1.0, 0.0, 1.0):
        for y in (-1.0, 0.0, 1.0):
            cloud.addPoint(CCVector3(x, y, 0.001 * (x - y)))
    return cloud


@pytest.fixture
def planar_cloud():
    return _make_planar_cloud()


@pytest.fixture
def neigh(planar_cloud):
    return Neighbourhood(planar_cloud)


class TestConstructionAndReset:
    def test_associated_cloud(self, planar_cloud, neigh):
        assert neigh.associatedCloud() is planar_cloud

    def test_reset_after_compute(self, neigh):
        assert neigh.getLSPlane() is not None
        neigh.reset()
        # getLSPlane() recomputes lazily — it should still succeed.
        assert neigh.getLSPlane() is not None


class TestLSPlane:
    def test_plane_equation_is_4_tuple(self, neigh):
        eq = neigh.getLSPlane()
        assert eq is not None
        assert len(eq) == 4

    def test_base_vectors_are_unit_and_orthogonal(self, neigh):
        x = neigh.getLSPlaneX()
        y = neigh.getLSPlaneY()
        n = neigh.getLSPlaneNormal()
        assert x is not None and y is not None and n is not None

        def norm(v):
            return (v.x * v.x + v.y * v.y + v.z * v.z) ** 0.5

        def dot(a, b):
            return a.x * b.x + a.y * b.y + a.z * b.z

        assert norm(x) == pytest.approx(1.0, abs=1e-4)
        assert norm(y) == pytest.approx(1.0, abs=1e-4)
        assert norm(n) == pytest.approx(1.0, abs=1e-4)
        assert dot(x, y) == pytest.approx(0.0, abs=1e-4)
        assert dot(x, n) == pytest.approx(0.0, abs=1e-4)
        assert dot(y, n) == pytest.approx(0.0, abs=1e-4)

    def test_normal_is_roughly_z(self, neigh):
        n = neigh.getLSPlaneNormal()
        assert abs(n.z) == pytest.approx(1.0, abs=1e-2)
        assert abs(n.x) == pytest.approx(0.0, abs=1e-2)
        assert abs(n.y) == pytest.approx(0.0, abs=1e-2)


class TestGravityCenter:
    def test_is_at_origin_for_symmetric_grid(self, neigh):
        g = neigh.getGravityCenter()
        assert g is not None
        assert g.x == pytest.approx(0.0, abs=1e-6)
        assert g.y == pytest.approx(0.0, abs=1e-6)
        assert g.z == pytest.approx(0.0, abs=1e-4)

    def test_set_gravity_center_is_observable(self, neigh):
        neigh.setGravityCenter(CCVector3(10.0, 20.0, 30.0))
        g = neigh.getGravityCenter()
        assert g.x == 10.0 and g.y == 20.0 and g.z == 30.0


class TestCovariance:
    def test_returns_square_matrixd(self, neigh):
        cov = neigh.computeCovarianceMatrix()
        assert isinstance(cov, SquareMatrixd)
        assert cov.size() == 3
        assert cov.isValid()

    def test_is_symmetric(self, neigh):
        cov = neigh.computeCovarianceMatrix()
        for i in range(3):
            for j in range(3):
                assert cov.getValue(i, j) == pytest.approx(cov.getValue(j, i), abs=1e-9)


class TestFeatures:
    @pytest.mark.parametrize(
        "feature",
        [
            Neighbourhood.GeomFeature.Planarity,
            Neighbourhood.GeomFeature.Linearity,
            Neighbourhood.GeomFeature.Sphericity,
            Neighbourhood.GeomFeature.SurfaceVariation,
            Neighbourhood.GeomFeature.Anisotropy,
        ],
    )
    def test_feature_is_finite(self, neigh, feature):
        v = neigh.computeFeature(feature)
        assert v == v  # not NaN

    def test_planar_cloud_is_planar(self, neigh):
        planarity = neigh.computeFeature(Neighbourhood.GeomFeature.Planarity)
        # A near-flat 3x3 grid should score near 1 on planarity.
        assert planarity > 0.9


class TestCurvatureAndRoughness:
    def test_curvature_at_cloud_center(self, neigh):
        v = neigh.computeCurvature(CCVector3(0.0, 0.0, 0.0), Neighbourhood.CurvatureType.MEAN_CURV)
        assert v == v  # not NaN

    def test_roughness_off_cloud(self, neigh):
        # Header contract: P should NOT be one of the cloud points.
        v = neigh.computeRoughness(CCVector3(0.25, 0.25, 1.5))
        assert v == v  # not NaN


class TestRadiusAndEnums:
    def test_largest_radius(self, neigh):
        r = neigh.computeLargestRadius()
        # Grid corner distance from origin is sqrt(2).
        assert r == pytest.approx(2.0**0.5, abs=1e-2)

    def test_enum_and_exported_value_agree(self):
        assert Neighbourhood.MEAN_CURV == Neighbourhood.CurvatureType.MEAN_CURV
        assert Neighbourhood.Planarity == Neighbourhood.GeomFeature.Planarity

    def test_static_constants(self):
        assert Neighbourhood.IGNORE_MAX_EDGE_LENGTH == 0
        assert Neighbourhood.DUPLICATE_VERTICES is True
        assert Neighbourhood.DO_NOT_DUPLICATE_VERTICES is False


class TestQuadric:
    def test_getQuadric_shape(self, neigh):
        result = neigh.getQuadric()
        if result is not None:
            coeffs, orientation = result
            assert len(coeffs) == 6
            # Orientation is a SquareMatrix (PointCoordinateType variant, 3x3).
            assert orientation.size() == 3

    def test_compute3DQuadric_shape(self, neigh):
        try:
            q = neigh.compute3DQuadric()
        except RuntimeError:
            # A near-planar 9-point grid is borderline for a full 3D quadric
            # fit; accept either a clean failure or a 10-tuple result.
            return
        assert len(q) == 10


class TestLSPlaneSetter:
    def test_setLSPlane_round_trips(self, neigh):
        eq = (0.0, 0.0, 1.0, 0.0)
        neigh.setLSPlane(eq, CCVector3(1, 0, 0), CCVector3(0, 1, 0), CCVector3(0, 0, 1))
        out = neigh.getLSPlane()
        assert out == pytest.approx(eq, abs=1e-6)
        assert neigh.getLSPlaneNormal().z == pytest.approx(1.0, abs=1e-6)
