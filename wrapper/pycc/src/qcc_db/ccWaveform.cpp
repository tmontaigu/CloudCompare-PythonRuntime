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
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <ccWaveform.h>

#include "../casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

void define_ccWaveform(py::module &m)
{
    py::class_<WaveformDescriptor>(m, "WaveformDescriptor")
        .def(py::init<>())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def_readwrite("numberOfSamples", &WaveformDescriptor::numberOfSamples)
        .def_readwrite("samplingRate_ps", &WaveformDescriptor::samplingRate_ps)
        .def_readwrite("digitizerGain", &WaveformDescriptor::digitizerGain)
        .def_readwrite("digitizerOffset", &WaveformDescriptor::digitizerOffset)
        .def_readwrite("bitsPerSample", &WaveformDescriptor::bitsPerSample);

    py::class_<ccWaveform>(m, "ccWaveform")
        .def(py::init<uint8_t>(), "id"_a)
        .def("descriptorID", &ccWaveform::descriptorID)
        .def("setDescriptorID", &ccWaveform::setDescriptorID, "id"_a)
        .def("setDataDescription", &ccWaveform::setDataDescription, "dataOffset"_a, "byteCount"_a)
        .def("getRawSample", &ccWaveform::getRawSample, "i"_a, "descriptor"_a, "dataStorage"_a)
        .def("getSample", &ccWaveform::getRawSample, "i"_a, "descriptor"_a, "dataStorage"_a)
        .def(
            "getRange",
            [](const ccWaveform &self,
               const WaveformDescriptor &descriptor,
               const uint8_t *dataStorage)
            {
                double min, max;
                self.getRange(min, max, descriptor, dataStorage);
                return py::make_tuple(min, max);
            },
            "descriptor"_a,
            "dataStorage"_a)
        .def(
            "decodeSamples",
            [](ccWaveform &self, const WaveformDescriptor &descriptor, const uint8_t *dataStorage)
            {
                std::vector<double> values;
                self.decodeSamples(values, descriptor, dataStorage);
                return values;
            },
            "descriptor"_a,
            "dataStorage"_a)
        .def("toASCII", &ccWaveform::toASCII, "filename"_a, "descriptor"_a, "dataStorage"_a)
        .def_static("ToASCII", &ccWaveform::ToASCII, "filename"_a, "values"_a, "samplingRate_ps"_a)
        .def("getSamplePos", &ccWaveform::getSamplePos, "i"_a, "getSamplePos"_a, "descriptor"_a)
        .def("byteCount", &ccWaveform::byteCount)
        .def("dataOffset", &ccWaveform::dataOffset)
        .def("setDataOffset", &ccWaveform::setDataOffset, "offset"_a)
        // TODO data(storage), the interaction with python will be weird to sort out
        .def("setBeamDir", &ccWaveform::setBeamDir, "dir"_a)
        .def("beamDir", &ccWaveform::beamDir)
        .def("setEchoTime_ps", &ccWaveform::setEchoTime_ps, "time_ps"_a)
        .def("echoTime_ps", &ccWaveform::echoTime_ps)
        .def("applyRigidTransformation", &ccWaveform::applyRigidTransformation, "trans"_a)
        .def("returnIndex", &ccWaveform::returnIndex)
        .def("setReturnIndex", &ccWaveform::setReturnIndex, "index"_a);

    py::class_<ccWaveformProxy>(m, "ccWaveformProxy")
        .def(py::init<const ccWaveform &, const WaveformDescriptor &, const uint8_t *>(),
             "w"_a,
             "d"_a,
             "storage"_a)
        .def("isValid", &ccWaveformProxy::isValid)
        .def("descriptorID", &ccWaveformProxy::descriptorID)
        .def("getRawSample", &ccWaveformProxy::getRawSample, "i"_a)
        .def("getSample", &ccWaveformProxy::getRawSample, "i"_a)
        .def("getRange",
             [](const ccWaveformProxy &self)
             {
                 double min, max;
                 self.getRange(min, max);
                 return py::make_tuple(min, max);
             })
        .def("decodeSamples",
             [](const ccWaveformProxy &self)
             {
                 std::vector<double> values;
                 self.decodeSamples(values);
                 return values;
             })
        .def("toASCII", &ccWaveformProxy::toASCII, "filename"_a)
        .def("getSamplePos", &ccWaveformProxy::getSamplePos, "i"_a, "getSamplePos"_a)
        .def("byteCount", &ccWaveformProxy::byteCount)
        .def("data", &ccWaveformProxy::data)
        .def("beamDir", &ccWaveformProxy::beamDir)
        .def("echoTime_ps", &ccWaveformProxy::echoTime_ps)
        .def("descriptor", &ccWaveformProxy::descriptor)
        .def("waveform", &ccWaveformProxy::waveform);
}
