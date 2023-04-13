import pycc

pc = pycc.GetInstance().getSelectedEntities()[0]

sf = pc.getScalarField(0)

# Get histogram values
hist = sf.getHistogram()
print(hist)

# Change the color scale displayed
scale = pycc.ccColorScalesManager.GetDefaultScale(pycc.ccColorScalesManager.YELLOW_BROWN)
sf.setColorScale(scale)
