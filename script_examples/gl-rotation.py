"""
This scripts applies a rotation of the first selected entity.
It's an OpenGL rotation, so coordinates are not changed, its just visual
"""

import pycc
import cccorelib
import math


entity = pycc.GetInstance().getSelectedEntities()[0]

# Rotating the entity around its center
# Rotation always happen in OpenGL`s origin which is probably not
# the center of our object
#
# So we have to:
# 1 - Translate the matrix by -center of the object
# 2 - Rotate the matrix by the desired amount
# 3 - Translate the matrix by centre of the object

center = entity.getDisplayBB_recursive(True).getCenter()

# 1
glTrans = entity.getGLTransformation()
translation = glTrans.getTranslationAsVec3D()
translation = translation - center
glTrans.setTranslation(translation)

entity.setGLTransformation(glTrans)
entity.applyGLTransformation_recursive()

# 2 & 3
glRot = pycc.ccGLMatrix()
glRot.initFromParameters(
    math.radians(45),
    cccorelib.CCVector3(0.0, 0.0, 1.0),
    cccorelib.CCVector3(0.0, 0.0, 0.0)
)

glMat = entity.getGLTransformation()
glMat = glMat * glRot;

translation = glMat.getTranslationAsVec3D()
translation = translation + center
glMat.setTranslation(translation)

entity.setGLTransformation(glMat)
entity.applyGLTransformation_recursive()

pycc.GetInstance().redrawAll()
