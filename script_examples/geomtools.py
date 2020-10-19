import cccorelib
import pycc

CC = pycc.GetInstance()


def main():
    entities = CC.getSelectedEntities()

    if not entities:
        raise RuntimeError("No entities selected")

    CC.freezeUI(True)
    pointCloud = entities[0]
    print(pointCloud)

    gravityCenter = cccorelib.GeometricalAnalysisTools.ComputeGravityCenter(pointCloud)
    print("The gravity center of {} is {}".format(pointCloud.getName(), gravityCenter))

    CC.freezeUI(False)


if __name__ == '__main__':
    main()
