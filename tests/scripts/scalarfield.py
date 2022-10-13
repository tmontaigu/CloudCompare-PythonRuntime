import cccorelib
import pycc
import unittest

cc = pycc.GetInstance()
cloud = cc.clouds()[0].pc
print(len(cc.clouds()))


class TestCase(unittest.TestCase):
    def run_test(self):
        self.assertEqual(cloud.size(), 10_683)

        self.assertEqual(cccorelib.ScalarFieldTools.computeMeanScalarValue(cloud), 8203.892578125)
        self.assertEqual(cccorelib.ScalarFieldTools.computeMeanSquareScalarValue(cloud), 93042544.0)
        self.assertEqual(cccorelib.ScalarFieldTools.computeScalarFieldGradient(cloud, 0, False), 0)

        self.assertEqual(cccorelib.ScalarFieldTools.countScalarFieldValidValues(cloud), 10_683)

        classificationSf = cloud.getScalarField(cloud.getScalarFieldIndexByName("Classification"))
        self.assertIsNotNone(classificationSf)
        self.assertEqual(classificationSf.getName(), "Classification")

        classificationSf.computeMinAndMax()

        self.assertEqual(classificationSf.getMax(), 11)
        self.assertEqual(classificationSf.getMin(), 11)

        classificationSf.fill(0)
        classificationSf.computeMinAndMax()

        self.assertEqual(classificationSf.getMax(), 0)
        self.assertEqual(classificationSf.getMin(), 0)

        classificationArray = classificationSf.asArray()
        classificationArray[:] = 17
        classificationSf.computeMinAndMax()

        self.assertEqual(classificationSf.getMax(), 17)
        self.assertEqual(classificationSf.getMin(), 17)


if __name__ == '__main__':
    TestCase().run_test()

