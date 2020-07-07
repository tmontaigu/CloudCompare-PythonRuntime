import pycc

CC = pycc.GetInstance()

params = pycc.FileIOFilter.LoadParameters()
obj = pycc.FileIOFilter.LoadFromFile(r"C:\Users\t.montaigu\Desktop\BDX_RG_270_0+0_0+50 - Cloud.bin", params)
print(obj)

CC.addToDB(obj)
