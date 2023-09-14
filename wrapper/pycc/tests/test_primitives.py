import pycc
import cccorelib


def test_cc_sphere():
    sphere = pycc.ccSphere(3.0)
    assert sphere.getTypeName() == "Sphere"
    assert sphere.getName() == "Sphere"
    assert sphere.getRadius() == 3.0
    sphere.setRadius(10.0)
    assert sphere.getRadius() == 10.0

    sphere2 = pycc.ccSphere(5.0, name="Sphere2")
    assert sphere2.getTypeName() == "Sphere"
    assert sphere2.getName() == "Sphere2"


def test_cc_cylinder():
    cylinder = pycc.ccCylinder(3.0, 10.0)
    assert cylinder.getTypeName() == "Cylinder"
    assert cylinder.getName() == "Cylinder"
    assert cylinder.getBottomRadius() == 3.0
    cylinder.setBottomRadius(10.0)
    assert cylinder.getBottomRadius() == 10.0

    cylinder2 = pycc.ccCylinder(radius=5.0, height=50, name="Cylinder2")
    assert cylinder2.getTypeName() == "Cylinder"
    assert cylinder2.getName() == "Cylinder2"


def test_cc_cone():
    cone = pycc.ccCone(10.0, 5.0, 20.0, name="SomeName")
    assert cone.getTypeName() == "Cone"
    assert cone.getName() == "SomeName"
    assert cone.getBottomRadius() == 10.0
    assert cone.getTopRadius() == 5.0
    assert cone.getHeight() == 20.0


def test_cc_plane():
    plane = pycc.ccPlane(10.0, 5.0, name="SomeName")
    assert plane.getTypeName() == "Plane"
    assert plane.getName() == "SomeName"
    assert plane.getXWidth() == 10.0
    assert plane.getYWidth() == 5.0


def test_cc_torus():
    torus = pycc.ccTorus(5.0, 10.0)
    assert torus.getTypeName() == "Torus"


def test_cc_boc():
    box = pycc.ccBox(cccorelib.CCVector3(5.0, 10.0, 4.0))
    assert box.getTypeName() == "Box"
    dims = box.getDimensions()
    assert dims.x == 5.0
    assert dims.y == 10.0
    assert dims.z == 4.0

    box.setDimensions(cccorelib.CCVector3(1.0, 2.0, 3.0))
    dims = box.getDimensions()
    assert dims.x == 1.0
    assert dims.y == 2.0
    assert dims.z == 3.0


def test_cc_dish():
    dish = pycc.ccDish(1.0, 4.0)
    assert dish.getTypeName() == "Dish"


def test_cc_quadric():
    quadric = pycc.ccQuadric(
        cccorelib.CCVector2(5.0, 10.0),
        cccorelib.CCVector2(10.0, 20.0),
        [1.0, 2.0, 3.0, 4.0, 5.0, 6.0]
    )
    assert quadric.getTypeName() == "Quadric"
