from cccorelib import ScalarField, ScalarFieldTools, PointCloud
import numpy as np


def test_histogram():
    point_cloud = PointCloud()
    point_cloud.resize(100)

    classification = np.zeros(len(point_cloud))
    classification[25:75] = 1
    classification[75:] = 2

    uniqs, counts = np.unique(classification, return_counts=True)

    idx = point_cloud.addScalarField("classification", classification)
    point_cloud.setCurrentScalarField(idx)
    histo = ScalarFieldTools.computeScalarFieldHistogram(point_cloud, 3)
    assert np.all(counts == histo)
