import pycc
import sys

CC = pycc.GetCmdLineInstance()

assert CC is not None

print("Number of loaded Clouds: {}".format(len(CC.clouds())))
print("Number of loaded Meshes: {}".format(len(CC.meshes())))

print(sys.argv)