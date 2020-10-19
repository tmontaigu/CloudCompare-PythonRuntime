import sys

import pycc

CC = pycc.GetCmdLineInstance()

assert CC is not None

print("Number of loaded Clouds: {}".format(len(CC.clouds())))
print("Number of loaded Meshes: {}".format(len(CC.meshes())))

print(sys.argv)

if len(sys.argv) > 2:
    CC.importFile(sys.argv[1])
