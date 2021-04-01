from typing import Iterable, Iterator, List

from typing import overload
import cccorelib

class FileIOFilter:
    LoadParameters: Any = ...
    def __init__(self, *args, **kwargs) -> None: ...
    def LoadFromFile(self, *args, **kwargs) -> Any: ...

class QFont:
    def __init__(self, *args, **kwargs) -> None: ...

class QMainWindow(QWidget):
    def __init__(self, *args, **kwargs) -> None: ...

class QOpenGLWidget:
    def __init__(self, *args, **kwargs) -> None: ...

class QPointF:
    def __init__(self, *args, **kwargs) -> None: ...

class QProgressDialog(QWidget):
    def __init__(self, *args, **kwargs) -> None: ...

class QSize:
    @overload
    def __init__(self) -> None: ...
    @overload
    def __init__(self, width: int, height: int) -> None: ...
    @overload
    def __init__(*args, **kwargs) -> Any: ...
    def height(self) -> int: ...
    def isEmpty(self) -> bool: ...
    def isValid(self) -> bool: ...
    def width(self) -> int: ...

class QWidget:
    def __init__(self, *args, **kwargs) -> None: ...

class Rgba:
    def __init__(self, *args, **kwargs) -> None: ...

class _NormsIndexesArrayType(_VectorCompressedNormType, cccorelib.CCShareable, ccHObject):
    def __init__(self, *args, **kwargs) -> None: ...

class _VectorCompressedNormType:
    __hash__: Any = ...
    __pybind11_module_local_v4_msvc__: Any = ...
    @overload
    def __init__(self) -> None: ...
    @overload
    def __init__(self, arg0: List[int]) -> None: ...
    @overload
    def __init__(self, arg0: Iterable) -> None: ...
    @overload
    def __init__(*args, **kwargs) -> Any: ...
    def append(self, x: int) -> None: ...
    def clear(self) -> None: ...
    def count(self, x: int) -> int: ...
    @overload
    def extend(self, L: List[int]) -> None: ...
    @overload
    def extend(self, L: Iterable) -> None: ...
    @overload
    def extend(*args, **kwargs) -> Any: ...
    def insert(self, i: int, x: int) -> None: ...
    @overload
    def pop(self) -> int: ...
    @overload
    def pop(self, i: int) -> int: ...
    @overload
    def pop(*args, **kwargs) -> Any: ...
    def remove(self, x: int) -> None: ...
    def __bool__(self) -> bool: ...
    def __contains__(self, x: int) -> bool: ...
    @overload
    def __delitem__(self, arg0: int) -> None: ...
    @overload
    def __delitem__(self, arg0: slice) -> None: ...
    @overload
    def __delitem__(*args, **kwargs) -> Any: ...
    def __eq__(self, arg0: List[int]) -> bool: ...
    @overload
    def __getitem__(self, s: slice) -> List[int]: ...
    @overload
    def __getitem__(self, arg0: int) -> int: ...
    @overload
    def __getitem__(*args, **kwargs) -> Any: ...
    def __iter__(self) -> Iterator: ...
    def __len__(self) -> int: ...
    def __ne__(self, arg0: List[int]) -> bool: ...
    @overload
    def __setitem__(self, arg0: int, arg1: int) -> None: ...
    @overload
    def __setitem__(self, arg0: slice, arg1: List[int]) -> None: ...
    @overload
    def __setitem__(*args, **kwargs) -> Any: ...

class __ccGenericPointCloudTpl(ccGenericPointCloud):
    def __init__(self, *args, **kwargs) -> None: ...
    def addPoint(self, P: cccorelib.CCVector3) -> None: ...
    def addScalarField(self, uniqueName: str) -> int: ...
    def deleteAllScalarFields(self) -> None: ...
    def deleteScalarField(self, index: int) -> None: ...
    def enableScalarField(self) -> bool: ...
    def forEach(self, action: Callable[[cccorelib.CCVector3,float],None]) -> None: ...
    def getBoundingBox(self, bbMin: cccorelib.CCVector3, bbMax: cccorelib.CCVector3) -> None: ...
    def getCurrentInScalarField(self) -> cccorelib.ScalarField: ...
    def getCurrentInScalarFieldIndex(self) -> int: ...
    def getCurrentOutScalarField(self) -> cccorelib.ScalarField: ...
    def getCurrentOutScalarFieldIndex(self) -> int: ...
    def getNextPoint(self) -> cccorelib.CCVector3: ...
    def getNumberOfScalarFields(self) -> int: ...
    def getPointScalarValue(self, pointIndex: int) -> float: ...
    def getScalarField(self, index: int) -> cccorelib.ScalarField: ...
    def getScalarFieldIndexByName(self, name: str) -> int: ...
    def getScalarFieldName(self, index: int) -> str: ...
    def invalidateBoundingBox(self) -> None: ...
    def isScalarFieldEnabled(self) -> bool: ...
    def renameScalarField(self, index: int, newName: str) -> bool: ...
    def reserve(self, newCapacity: int) -> bool: ...
    def reset(self) -> None: ...
    def resize(self, newCount: int) -> bool: ...
    def setCurrentInScalarField(self, index: int) -> None: ...
    def setCurrentOutScalarField(self, index: int) -> None: ...
    def setCurrentScalarField(self, index: int) -> None: ...
    def setPointScalarValue(self, pointIndex: int, value: float) -> None: ...
    def size(self) -> int: ...

class cc2DLabel(ccHObject, ccInteractor):
    PickedPoint: Any = ...
    def __init__(self, name: QString = ...) -> None: ...
    @overload
    def addPickedPoint(self, cloud: ccGenericPointCloud, pointIndex: int, entityCenter: bool = ...) -> bool: ...
    @overload
    def addPickedPoint(self, cloud: ccGenericMesh, triangleIndex: int, uv: cccorelib.CCVector2d, entityCenter: bool = ...) -> bool: ...
    @overload
    def addPickedPoint(self, pp: cc2DLabel.PickedPoint) -> bool: ...
    @overload
    def addPickedPoint(*args, **kwargs) -> Any: ...
    def clear(self, ignoreDependencies: bool = ...) -> None: ...
    def displayPointLegend(self, state: bool) -> None: ...
    def getLabelContent(self, precision: int) -> QStringList: ...
    def getPickedPoint(self, index: int) -> cc2DLabel.PickedPoint: ...
    def getRawName(self) -> QString: ...
    def getTitle(self, precision: int) -> QString: ...
    def isCollapsed(self) -> bool: ...
    def isDisplayedIn2D(self) -> bool: ...
    def isPointLegendDisplayed(self) -> bool: ...
    def setCollapsed(self, state: bool) -> None: ...
    def setDisplayedIn2D(self, state: bool) -> None: ...
    def setPosition(self, x: float, y: float) -> None: ...
    def setRelativeMarkerScale(self, scale: float) -> None: ...
    def size(self) -> int: ...

class ccBox(ccGenericPrimitive):
    def __init__(self, dims: cccorelib.CCVector3, transMat: ccGLMatrix = ..., name: QString = ...) -> None: ...
    def getDimensions(self) -> cccorelib.CCVector3: ...
    def setDimensions(self, dims: cccorelib.CCVector3) -> None: ...

class ccCone(ccGenericPrimitive):
    def __init__(self, bottomRadius: float, topRadius: float, height: float, xOff: float = ..., yOff: float = ..., transMat: ccGLMatrix = ..., name: QString = ..., precision: int = ..., uniqueID: int = ...) -> None: ...
    def getBottomCenter(self) -> cccorelib.CCVector3: ...
    def getBottomRadius(self) -> float: ...
    def getHeight(self) -> float: ...
    def getLargeCenter(self) -> cccorelib.CCVector3: ...
    def getLargeRadius(self) -> float: ...
    def getSmallCenter(self) -> cccorelib.CCVector3: ...
    def getSmallRadius(self) -> float: ...
    def getTopCenter(self) -> cccorelib.CCVector3: ...
    def getTopRadius(self) -> float: ...
    def isSnoutMode(self) -> bool: ...
    def setBottomRadius(self, radius: float) -> None: ...
    def setHeight(self, height: float) -> None: ...
    def setTopRadius(self, radius: float) -> None: ...

class ccCylinder(ccCone):
    def __init__(self, radius: float, height: float, transMat: ccGLMatrix = ..., name: QString = ..., precision: int = ..., uniqueID: int = ...) -> None: ...

class ccDish(ccGenericPrimitive):
    def __init__(self, radius: float, height: float, radius2: float = ..., transMat: ccGLMatrix = ..., name: QString = ..., precision: int = ...) -> None: ...

class ccDrawableObject:
    def __init__(self, *args, **kwargs) -> None: ...
    def addClipPlanes(self, plane: ccClipPlane) -> bool: ...
    def colorsShown(self) -> bool: ...
    def draw(self, context: ccGLDrawContext) -> None: ...
    def enableGLTransformation(self, state: bool) -> None: ...
    def getDisplay(self) -> ccGenericGLDisplay: ...
    def getGLTransformation(self) -> ccGLMatrix: ...
    def getTempColor(self) -> Rgba: ...
    def hasColors(self) -> bool: ...
    def hasDisplayedScalarField(self) -> bool: ...
    def hasNormals(self) -> bool: ...
    def hasScalarFields(self) -> bool: ...
    def isColorOverriden(self) -> bool: ...
    def isGLTransEnabled(self) -> bool: ...
    def isSelected(self) -> bool: ...
    def isVisibilityLocked(self) -> bool: ...
    def isVisible(self) -> bool: ...
    def lockVisibility(self, arg0: bool) -> None: ...
    def nameShownIn3D(self) -> bool: ...
    def normalsShown(self) -> bool: ...
    def prepareDisplayForRefresh(self) -> None: ...
    def redrawDisplay(self) -> None: ...
    def refreshDisplay(self, only2D: bool = ...) -> None: ...
    def removeAllClipPlanes(self) -> None: ...
    def removeFromDisplay(self, win: ccGenericGLDisplay) -> None: ...
    def resetGLTransformation(self) -> None: ...
    def rotateGL(self, rotMat: ccGLMatrix) -> None: ...
    def setDisplay(self, win: ccGenericGLDisplay) -> None: ...
    def setGLTransformation(self, trans: ccGLMatrix) -> None: ...
    def setSelected(self, state: bool) -> None: ...
    @overload
    def setTempColor(self, col: Rgba, autoActivate: bool = ...) -> None: ...
    @overload
    def setTempColor(self, col, autoActivate: bool = ...) -> None: ...
    @overload
    def setTempColor(*args, **kwargs) -> Any: ...
    def setVisible(self, state: bool) -> None: ...
    def sfShown(self) -> bool: ...
    def showColors(self, state: bool) -> None: ...
    def showNameIn3D(self, state: bool) -> None: ...
    def showNormals(self, state: bool) -> None: ...
    def showSF(self, state: bool) -> None: ...
    def toggleClipPlanes(self, context: ccGLDrawContext, enable: bool) -> None: ...
    def toggleColors(self) -> None: ...
    def toggleMaterials(self) -> None: ...
    def toggleNormals(self) -> None: ...
    def toggleSF(self) -> None: ...
    def toggleShowName(self) -> None: ...
    def toggleVisibility(self) -> None: ...
    def translateGL(self, rotMat: cccorelib.CCVector3) -> None: ...

class ccGLCameraParameters:
    def __init__(self) -> None: ...
    @overload
    def project(self, input3D: cccorelib.CCVector3d, output2D: cccorelib.CCVector3d, checkInFrustrum: bool = ...) -> bool: ...
    @overload
    def project(self, input3D: cccorelib.CCVector3, output2D: cccorelib.CCVector3d, checkInFrustrum: bool = ...) -> bool: ...
    @overload
    def project(*args, **kwargs) -> Any: ...
    @overload
    def unproject(self, input3D: cccorelib.CCVector3d, output2D: cccorelib.CCVector3d) -> bool: ...
    @overload
    def unproject(self, input3D: cccorelib.CCVector3, output2D: cccorelib.CCVector3d) -> bool: ...
    @overload
    def unproject(*args, **kwargs) -> Any: ...
    @property
    def fov_deg(self) -> float: ...
    @fov_deg.setter
    def fov_deg(self, val: float) -> None: ...
    @property
    def modelViewMat(self) -> ccGLMatrixd: ...
    @modelViewMat.setter
    def modelViewMat(self, val: ccGLMatrixd) -> None: ...
    @property
    def perspective(self) -> bool: ...
    @perspective.setter
    def perspective(self, val: bool) -> None: ...
    @property
    def pixelSize(self) -> float: ...
    @pixelSize.setter
    def pixelSize(self, val: float) -> None: ...
    @property
    def projectionMat(self) -> ccGLMatrixd: ...
    @projectionMat.setter
    def projectionMat(self, val: ccGLMatrixd) -> None: ...

class ccGLMatrix:
    def __init__(self, *args, **kwargs) -> None: ...

class ccGLWindow(QOpenGLWidget, ccGenericGLDisplay):
    def __init__(self, *args, **kwargs) -> None: ...

class ccGenericGLDisplay:
    TextAlign: Any = ...
    def __init__(self, *args, **kwargs) -> None: ...
    def deprecate3DLayer(self) -> None: ...
    def display3DLabel(self, str: QString, pos3D: cccorelib.CCVector3, color: Rgba = ..., font: QFont = ...) -> None: ...
    def displayText(self, text: QString, x: int, y: int, align: int = ..., bkgAlpha: float = ..., color: Rgba = ..., font: QFont = ...) -> None: ...
    def getGLCameraParameters(self, params: ccGLCameraParameters) -> None: ...
    def getLabelDisplayFont(self) -> QFont: ...
    def getScreenSize(self) -> QSize: ...
    def getTextDisplayFont(self) -> QFont: ...
    def invalidateViewport(self) -> None: ...
    def redraw(self, only2D: bool = ..., resetLOD: bool = ...) -> None: ...
    def refresh(self, only2D: bool = ...) -> None: ...
    def toBeRefreshed(self) -> None: ...
    def toCenteredGLCoordinates(self, x: int, y: int) -> QPointF: ...

class ccGenericMesh(cccorelib.GenericIndexedMesh, ccHObject):
    def __init__(self, *args, **kwargs) -> None: ...
    def capacity(self) -> int: ...
    def getAssociatedCloud(self) -> ccGenericPointCloud: ...
    def getMaterialSet(self) -> ccMaterialSet: ...
    def getTexCoordinatesTable(self) -> TextureCoordsContainer: ...
    def getTriangleMtlIndex(self, triangleIndex: int) -> int: ...
    def getTriangleNormalIndexes(self, triangleIndex: int) -> tuple: ...
    def getTriangleNormals(self, triangleIndex: int) -> tuple: ...
    def getTriangleTexCoordinates(self, triIndex: int) -> tuple: ...
    def getTriangleTexCoordinatesIndexes(self, triangleIndex: int) -> tuple: ...
    def hasMaterials(self) -> bool: ...
    def hasPerTriangleTexCoordinates(self) -> bool: ...
    def hasTextures(self) -> bool: ...
    def hasTriNormals(self) -> bool: ...
    def refreshBB(self) -> None: ...

class ccGenericPointCloud(ccShiftedObject, cccorelib.GenericIndexedCloudPersist):
    def __init__(self, *args, **kwargs) -> None: ...

class ccGenericPrimitive(ccMesh):
    def __init__(self, *args, **kwargs) -> None: ...
    def clone(self) -> QString: ...
    def getDrawingPrecision(self) -> int: ...
    def getTransformation(self) -> ccGLMatrix: ...
    def getTypeName(self) -> QString: ...
    def hasDrawingPrecision(self) -> bool: ...
    def setColor(self, col) -> None: ...
    def setDrawingPrecision(self, steps: int) -> bool: ...

class ccGlobalShiftManager:
    ALWAYS_DISPLAY_DIALOG: Any = ...
    DIALOG_IF_NECESSARY: Any = ...
    Mode: Any = ...
    NO_DIALOG: Any = ...
    NO_DIALOG_AUTO_SHIFT: Any = ...
    def __init__(self, *args, **kwargs) -> None: ...

class ccHObject(ccObject, ccDrawableObject):
    def __init__(self, *args, **kwargs) -> None: ...
    def find(self, arg0: int) -> ccHObject: ...
    def getChild(self, arg0: int) -> ccHObject: ...
    def getChildCountRecursive(self) -> int: ...
    def getChildrenNumber(self) -> int: ...
    def getParent(self) -> ccHObject: ...
    def isGroup(self) -> bool: ...

class ccInteractor:
    def __init__(self, *args, **kwargs) -> None: ...
    def acceptClick(self, x: int, y: int, button) -> bool: ...
    def move2D(self, x: int, y: int, dx: int, dy: int, screenWidth: int, screenHeight: int) -> bool: ...
    def move3D(self, u: cccorelib.CCVector3d) -> bool: ...

class ccLog:
    MessageLevelFlags: Any = ...
    def __init__(self, *args, **kwargs) -> None: ...
    def Error(self, *args, **kwargs) -> Any: ...
    def LogMessage(self, *args, **kwargs) -> Any: ...
    def Print(self, *args, **kwargs) -> Any: ...
    def TheInstance(self, *args, **kwargs) -> Any: ...
    def Warning(self, *args, **kwargs) -> Any: ...

class ccMesh(ccGenericMesh):
    def __init__(self, *args, **kwargs) -> None: ...
    def setAssociatedCloud(self, cloud: ccGenericPointCloud) -> None: ...

class ccObject:
    def __init__(self, *args, **kwargs) -> None: ...
    def getName(self) -> QString: ...
    def getUniqueID(self) -> int: ...
    def isCustom(self) -> bool: ...
    def isEnabled(self) -> bool: ...
    def isHierarchy(self) -> bool: ...
    def isLeaf(self) -> bool: ...
    def isLocked(self) -> bool: ...
    def setEnabled(self, arg0: bool) -> None: ...
    def setLocked(self, arg0: bool) -> None: ...
    def setName(self, arg0: QString) -> None: ...
    def toggleActivation(self) -> None: ...

class ccPlanarEntityInterface:
    def __init__(self, *args, **kwargs) -> None: ...
    def getNormal(self) -> cccorelib.CCVector3: ...
    def normalVectorIsShown(self) -> bool: ...
    def showNormalVector(self, state: bool) -> None: ...

class ccPlane(ccGenericPrimitive, ccPlanarEntityInterface):
    def __init__(self, xWidth: float, yWidth: float, transMat: ccGLMatrix = ..., name: QString = ...) -> None: ...
    def Fit(self, *args, **kwargs) -> Any: ...
    def flip(self) -> None: ...
    def getCenter(self) -> cccorelib.CCVector3: ...
    def getEquation(self) -> tuple: ...
    def getXWidth(self) -> float: ...
    def getYWidth(self) -> float: ...
    def setXWidth(self, w: float, autoUpdate: bool = ...) -> None: ...
    def setYWidth(self, h: float, autoUpdate: bool = ...) -> None: ...

class ccPointCloud(__ccGenericPointCloudTpl):
    def __init__(self, name: QString = ..., uniqueID: int = ...) -> None: ...
    def getCurrentDisplayedScalarField(self) -> ccScalarField: ...
    def getCurrentDisplayedScalarFieldIndex(self) -> int: ...
    def partialClone(self, reference: cccorelib.ReferenceCloud, warnings: int = ...) -> ccPointCloud: ...
    def reserveTheNormsTable(self) -> bool: ...
    def reserveThePointsTable(self, _numberOfPoints: int) -> bool: ...
    @overload
    def reserveTheRGBTable(self, arg0: int) -> bool: ...
    @overload
    def reserveTheRGBTable(self, fillWithWhite: int = ...) -> bool: ...
    @overload
    def reserveTheRGBTable(*args, **kwargs) -> Any: ...
    def resizeTheNormsTable(self) -> bool: ...
    def setCurrentDisplayedScalarField(self, index: int) -> None: ...
    def sfColorScaleShown(self) -> bool: ...
    def showSFColorsScale(self, state: bool) -> None: ...
    def shrinkToFit(self) -> None: ...

class ccProgressDialog(QProgressDialog, cccorelib.GenericProgressCallback):
    def __init__(self, cancelButton: bool = ...) -> None: ...
    def setInfo(self, infoStr: QString) -> None: ...
    def setMethodTitle(self, methodTitle: QString) -> None: ...

class ccScalarField(cccorelib.ScalarField):
    def __init__(self, *args, **kwargs) -> None: ...

class ccShiftedObject(ccHObject):
    def __init__(self, *args, **kwargs) -> None: ...
    def getGlobalScale(self, arg0: float) -> None: ...
    def getGlobalShift(self) -> cccorelib.CCVector3d: ...
    def isShifted(self) -> bool: ...
    def setGlobalScale(self, scale: float) -> None: ...
    @overload
    def setGlobalShift(self, x_: float, y: float, z: float) -> None: ...
    @overload
    def setGlobalShift(self, shift: cccorelib.CCVector3d) -> None: ...
    @overload
    def setGlobalShift(*args, **kwargs) -> Any: ...

class ccSphere(ccGenericPrimitive):
    def __init__(self, radius: float, transMat: ccGLMatrix = ..., name: QString = ..., precision: int = ..., uniqueId: int = ...) -> None: ...
    def getRadius(self) -> float: ...
    def setRadius(self, radius: float) -> None: ...

class ccTorus(ccGenericPrimitive):
    def __init__(self, insideRadius: float, outsideRadius: float, angleRad: float = ..., rectangularSection: bool = ..., rectSectionHeight: float = ..., transMat: ccGLMatrix = ..., name: QString = ..., precision: int = ..., uniqueId: int = ...) -> None: ...
from typing import Union, List

class ccPythonInstance:
    def haveSelection(self) -> bool: ...

    def haveOneSelection(self) -> bool: ...

    def getSelectedEntites(self) -> List[Union[ccHObject, ccPointCloud, ccMesh]]: ...


def GetInstance() -> ccPythonInstance: ...
