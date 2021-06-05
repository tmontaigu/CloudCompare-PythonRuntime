from cccorelib import ScalarField, ScalarFieldTools


def test_histogram():
    sf = ScalarField("Classification")
    sf.resizeSafe(100)
    sf.asArray()[:50] = 2

    # ScalarFieldTools.computeScalarFieldHistogram()