import cccorelib
import numpy as np
import pycc

d = cccorelib.NormalDistribution(1.0, 0.5)

mu, sigma2 = d.getParameters()
assert mu == 1.0 == d.getMu()
assert sigma2 == 0.5 == d.getSigma2()

d.setParameters(0.0, 0.0)

mu, sigma2 = d.getParameters()
assert mu == 0.0 == d.getMu()
assert sigma2 == 0.0 == d.getSigma2()

CC = pycc.GetCmdLineInstance()
cloud = CC.clouds()[0].pc

d.computeParameters(cloud)

d.setParameters(1.1, 0.6)
assert d.getMu() == float(np.float32(1.1))
assert d.getSigma2() == float(np.float32(0.6))
