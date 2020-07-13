import cccorelib

d = cccorelib.NormalDistribution(1.0, 0.5)
mu, sigma2 = d.getParameters()
assert mu == 1.0 == d.getMu()
assert sigma2 == 0.5 == d.getSigma2()


