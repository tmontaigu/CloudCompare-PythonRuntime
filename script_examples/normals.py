import pycc
import cccorelib

pc = pycc.GetInstance().getSelectedEntities()[0]


progress = pycc.ccProgressDialog()
progress.start()

success = pc.computeNormalsWithOctree(
    cccorelib.LOCAL_MODEL_TYPES.LS,
    pycc.ccNormalVectors.Orientation.UNDEFINED,
    5.0,
    pDlg=progress,
)
if not success:
    raise RuntimeError("Failed to compute normals")

kNN = 6
success = pc.orientNormalsWithMST(6, progress)
if not success:
    raise RuntimeError("Failed to orient normals")
