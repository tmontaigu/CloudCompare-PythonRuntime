// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonRuntime                       #
// #                                                                        #
// #  This program is free software; you can redistribute it and/or modify  #
// #  it under the terms of the GNU General Public License as published by  #
// #  the Free Software Foundation; version 2 of the License.               #
// #                                                                        #
// #  This program is distributed in the hope that it will be useful,       #
// #  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
// #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
// #  GNU General Public License for more details.                          #
// #                                                                        #
// #                   COPYRIGHT: Thomas Montaigu                           #
// #                                                                        #
// ##########################################################################

#include <pybind11/pybind11.h>

#include <ccCameraSensor.h>
#include <ccMesh.h>
#include <ccPointCloud.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccCameraSensor(py::module &m)
{
    py::class_<ccCameraSensor, ccSensor> pyccCameraSensor(m, "ccCameraSensor");

    py::class_<ccCameraSensor::IntrinsicParameters>(pyccCameraSensor, "IntrinsicParameters")
        .def(py::init<>())
        .def_static("GetKinectDefaults",
                    &ccCameraSensor::IntrinsicParameters::GetKinectDefaults,
                    "params"_a)
        .def_readwrite("vertFocal_pix", &ccCameraSensor::IntrinsicParameters::vertFocal_pix)
        .def_property(
            "pixelSize_mm",
            [](const ccCameraSensor::IntrinsicParameters &self) -> py::tuple
            { return py::make_tuple(self.pixelSize_mm[0], self.pixelSize_mm[1]); },
            [](ccCameraSensor::IntrinsicParameters &self, const py::tuple &pixelSize)
            {
                auto x = pixelSize[0].cast<float>();
                auto y = pixelSize[1].cast<float>();
                self.pixelSize_mm[0] = x;
                self.pixelSize_mm[1] = y;
            })
        .def_readwrite("skew", &ccCameraSensor::IntrinsicParameters::skew)
        .def_readwrite("vFOV_rad", &ccCameraSensor::IntrinsicParameters::vFOV_rad)
        .def_readwrite("zNear_mm", &ccCameraSensor::IntrinsicParameters::zNear_mm)
        .def_readwrite("zFar_mm", &ccCameraSensor::IntrinsicParameters::zFar_mm)
        .def_readwrite("arrayWidth", &ccCameraSensor::IntrinsicParameters::arrayWidth)
        .def_readwrite("arrayHeight", &ccCameraSensor::IntrinsicParameters::arrayHeight)
        .def("horizFocal_pix", &ccCameraSensor::IntrinsicParameters::horizFocal_pix);

    py::enum_<ccCameraSensor::DistortionModel>(pyccCameraSensor, "DistortionModel")
        .value("NO_DISTORTION_MODEL", ccCameraSensor::DistortionModel::NO_DISTORTION_MODEL)
        .value("SIMPLE_RADIAL_DISTORTION",
               ccCameraSensor::DistortionModel::SIMPLE_RADIAL_DISTORTION)
        .value("BROWN_DISTORTION", ccCameraSensor::DistortionModel::BROWN_DISTORTION)
        .value("EXTENDED_RADIAL_DISTORTION",
               ccCameraSensor::DistortionModel::EXTENDED_RADIAL_DISTORTION)
        .export_values();

    py::class_<ccCameraSensor::LensDistortionParameters>(pyccCameraSensor,
                                                         "LensDistortionParameters")
        .def("getModel", &ccCameraSensor::LensDistortionParameters::getModel);

    py::class_<ccCameraSensor::RadialDistortionParameters,
               ccCameraSensor::LensDistortionParameters>(pyccCameraSensor,
                                                         "RadialDistortionParameters")
        .def_readwrite("k1", &ccCameraSensor::RadialDistortionParameters::k1)
        .def_readwrite("k2", &ccCameraSensor::RadialDistortionParameters::k2);

    py::class_<ccCameraSensor::ExtendedRadialDistortionParameters,
               ccCameraSensor::RadialDistortionParameters>(pyccCameraSensor,
                                                           "ExtendedRadialDistortionParameters")
        .def_readwrite("k3", &ccCameraSensor::ExtendedRadialDistortionParameters::k3);

    py::class_<ccCameraSensor::BrownDistortionParameters, ccCameraSensor::LensDistortionParameters>(
        pyccCameraSensor, "BrownDistortionParameters")
        .def_property(
            "principalPointOffset",
            [](const ccCameraSensor::BrownDistortionParameters &self) -> py::tuple
            { return py::make_tuple(self.principalPointOffset[0], self.principalPointOffset[1]); },
            [](ccCameraSensor::BrownDistortionParameters &self,
               const py::tuple &principalPointOffset)
            {
                auto x = principalPointOffset[0].cast<float>();
                auto y = principalPointOffset[1].cast<float>();
                self.principalPointOffset[0] = x;
                self.principalPointOffset[1] = y;
            })
        .def_property(
            "linearDisparityParams",
            [](const ccCameraSensor::BrownDistortionParameters &self) -> py::tuple {
                return py::make_tuple(self.linearDisparityParams[0], self.linearDisparityParams[1]);
            },
            [](ccCameraSensor::BrownDistortionParameters &self,
               const py::tuple &linearDisparityParams)
            {
                auto x = linearDisparityParams[0].cast<float>();
                auto y = linearDisparityParams[1].cast<float>();
                self.linearDisparityParams[0] = x;
                self.linearDisparityParams[1] = y;
            })
        .def_property(
            "K_BrownParams",
            [](const ccCameraSensor::BrownDistortionParameters &self) -> py::tuple {
                return py::make_tuple(
                    self.K_BrownParams[0], self.K_BrownParams[1], self.K_BrownParams[2]);
            },
            [](ccCameraSensor::BrownDistortionParameters &self, const py::tuple &K_BrownParams)
            {
                auto x = K_BrownParams[0].cast<float>();
                auto y = K_BrownParams[1].cast<float>();
                auto z = K_BrownParams[2].cast<float>();

                self.K_BrownParams[0] = x;
                self.K_BrownParams[1] = y;
                self.K_BrownParams[2] = z;
            })
        .def_property(
            "P_BrownParams",
            [](const ccCameraSensor::BrownDistortionParameters &self) -> py::tuple
            { return py::make_tuple(self.P_BrownParams[0], self.P_BrownParams[1]); },
            [](ccCameraSensor::BrownDistortionParameters &self, const py::tuple &P_BrownParams)
            {
                auto x = P_BrownParams[0].cast<float>();
                auto y = P_BrownParams[1].cast<float>();
                self.P_BrownParams[0] = x;
                self.P_BrownParams[1] = y;
            });

    py::class_<ccCameraSensor::FrustumInformation>(pyccCameraSensor, "FrustumInformation")
        .def(py::init<>())
        .def("initFrustumCorners", &ccCameraSensor::FrustumInformation::initFrustumCorners)
        .def("initFrustumHull", &ccCameraSensor::FrustumInformation::initFrustumHull)
        .def_readwrite("isComputed", &ccCameraSensor::FrustumInformation::isComputed)
        .def_readwrite("drawFrustum", &ccCameraSensor::FrustumInformation::drawFrustum)
        .def_readwrite("drawSidePlanes", &ccCameraSensor::FrustumInformation::drawSidePlanes)
        .def_readwrite("frustumCorners",
                       &ccCameraSensor::FrustumInformation::frustumCorners,
                       py::return_value_policy::reference)
        .def_readwrite("frustumHull",
                       &ccCameraSensor::FrustumInformation::frustumHull,
                       py::return_value_policy::reference)
        .def_readwrite("center", &ccCameraSensor::FrustumInformation::center);

    pyccCameraSensor
        .def(py::init<ccCameraSensor::IntrinsicParameters>())
        // getters and setters
        .def("setVertFocal_pix", &ccCameraSensor::setVertFocal_pix, "vertFocal_pix"_a)
        .def("getHorizFocal_pix", &ccCameraSensor::getHorizFocal_pix)
        .def("getHorizFocal_pix", &ccCameraSensor::getHorizFocal_pix)
        .def("setVerticalFov_rad", &ccCameraSensor::setVerticalFov_rad, "fov_rad"_a)
        .def("getVerticalFov_rad", &ccCameraSensor::getVerticalFov_rad)
        .def("getIntrinsicParameters", &ccCameraSensor::getIntrinsicParameters)
        .def("setIntrinsicParameters", &ccCameraSensor::setIntrinsicParameters, "params"_a)
        .def(
            "getIntrinsicParameters",
            [](const ccCameraSensor &self) -> ccCameraSensor::LensDistortionParameters *
            { return self.getDistortionParameters().data(); },
            py::return_value_policy::reference);
    // TODO setDistortionParameters
    // TODO getProjectionMatrix and below
}
