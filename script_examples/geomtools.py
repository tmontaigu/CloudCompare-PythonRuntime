import pycc
import numpy as np

CC = pycc.GetInstance()

def main():
    entities = CC.getSelectedEntities()
    print(entities)

    if not entities:
        raise RuntimeError("No entities selected")


    CC.freezeUI(True)
    pc = entities[0]
    print(pc, pc.size())

    gravityCenter = pycc.GeometricalAnalysisTools.ComputeGravityCenter(pc)
    print("The gravity center of {} is {}".format(pc.getName(), gravityCenter))

    CC.freezeUI(False)
  


if __name__ == '__main__':
    main()
