"""
This scripts applies a translation of the first selected entity.
It's an OpenGL translation, so coordinates are not changed, its just visual
"""
import pycc
import cccorelib
import math


entity = pycc.GetInstance().getSelectedEntities()[0]

# Translating the entity

glMat = entity.getGLTransformation()
translation = glMat.getTranslationAsVec3D()
translation.x += 10.0
glMat.setTranslation(translation)

entity.setGLTransformation(glMat)
entity.applyGLTransformation_recursive()
pycc.GetInstance().redrawAll()
