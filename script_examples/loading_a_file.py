import pycc

CC = pycc.GetInstance()

params = pycc.FileIOFilter.LoadParameters()
params.parentWidget = CC.getMainWindow()


obj = CC.loadFile(r"C:\Users\t.montaigu\Projects\CloudCompare\plugins\private\CloudCompare-PythonPlugin\tests\data\a_cloud.bin", params)


obj = pycc.FileIOFilter.LoadFromFile(r"C:\Users\t.montaigu\Desktop\BDX_RG_270_0+0_0+50 - Cloud.bin", params)

if obj is not None:
    CC.addToDB(obj)
