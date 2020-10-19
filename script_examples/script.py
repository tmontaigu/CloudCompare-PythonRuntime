import numpy as np
import pycc

CC = pycc.GetInstance()


def main():
    print("hello world")

    hierObj = CC.loadFile(r"C:\Users\Thomas\Desktop\building_01_door_01.shp")
    print(hierObj, "is hierarchy obj:", hierObj.isHierarchy(), "is enabled ?:", hierObj.isEnabled())

    obj = hierObj.getChild(0)
    obj.setEnabled(False)
    assert obj.isEnabled() == False
    obj.setEnabled(True)
    assert obj.isEnabled() == True

    if not obj.nameShownIn3D():
        obj.showNameIn3D(True)

    CC.setSelectedInDB(obj, True)

    entities = CC.getSelectedEntities()
    print(entities)

    if not entities:
        raise RuntimeError("No entities selected")
        return

    pc = entities[0]
    print(f"The point cloud has {pc.size()} points")
    print(pc.getName())
    pc.setName("Renamed from python")

    sf = pc.getScalarField(0).asArray()
    print(pc.getScalarField(0).getName())
    print(f"{sf}")
    sf[:] = 42.0
    print(f"{sf}")

    try:
        pc.getScalarField(157)
    except IndexError:
        pass
    else:
        assert False


def main2():
    point = pycc.Vector3(1.0, 2.0, 3.0)
    print(f"First Point: {point}")

    point.x = 17.256
    point.y = 12.5
    point.z = 42.65
    assert np.isclose(point.x, 17.256)
    assert np.isclose(point.y, 12.5)
    assert np.isclose(point.z, 42.65)

    point = pycc.Vector3(25.0, 15.0, 30.0)
    point2 = pycc.Vector3(5.0, 5.0, 5.0)

    p = point - point2


if __name__ == '__main__':
    main()
    main2()
