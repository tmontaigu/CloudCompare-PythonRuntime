import pycc
import numpy as np


def main():
    pycc.PrintMessage("Starting")

    entities = pycc.GetSelectedEntities()
    pycc.PrintMessage(str(entities))

    if not entities:
        pycc.PrintError("No entities were selected")
        return

    pc = entities[0]
    pycc.PrintMessage(f"The point cloud has {pc.size()} points")
    pc.setName("Renamed from python")

    sf = pc.getScalarField(0).asArray()
    pycc.PrintMessage(pc.getScalarField(0).getName());
    pycc.PrintMessage(f"{sf}")
    sf[:] = 42.0
    pycc.PrintMessage(f"{sf}")

    try:
        pc.getScalarField(157)
    except IndexError:
        pass
    else:
        assert False


if __name__ == '__main__':
    main()