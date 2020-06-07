//##########################################################################
//#                                                                        #
//#                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                             COPYRIGHT: Thomas Montaigu                 #
//#                                                                        #
//##########################################################################

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <ccHObject.h>
#include <ccHObjectCaster.h>
#include <ccPointCloud.h>

#include "exposed.h"


namespace py = pybind11;

template<class T>
class NonOwning {
public:
    explicit NonOwning(T *ptr) : m_ptr(ptr) {};

    ~NonOwning() = default;

    T *get() { return m_ptr; }

    const T *get() const { return m_ptr; };

private:
    T *m_ptr;
};

class ScalarField {
public:
    explicit ScalarField(CCCoreLib::ScalarField *ptr) : m_scalarField(ptr) {};

    py::array_t<ScalarType> asArray() {
        // https://stackoverflow.com/questions/44659924/returning-numpy-arrays-via-pybind11
        // https://github.com/pybind/pybind11/issues/1042
        auto capsule = py::capsule(m_scalarField->data(), [](void *v) {
            // We don't delete, as CloudCompare C++ side is owning this data
        });
        return py::array(m_scalarField->size(), m_scalarField->data(), capsule);
    }

    const char *getName() const {
        return m_scalarField->getName();
    }

    void setName(const char *name) {
        m_scalarField->setName(name);
    }

    ~ScalarField() = default;

private:
    CCCoreLib::ScalarField *m_scalarField;
};



//PYBIND11_DECLARE_HOLDER_TYPE(T, NonOwning < T >);

PYBIND11_MODULE(pycc, m) {
    m.doc() = R"pbdoc(
        Python module exposing some CloudCompare functions
    )pbdoc";


    m.def("GetSelectedEntities", &GetSelectedEntities, py::return_value_policy::automatic_reference,
          R"pbdoc(Returns the list of currently selected entities in the CloudCompare DB Tree)pbdoc");

    m.def("PrintMessage", &PrintMessage);
    m.def("PrintWarning", &PrintWarning);
    m.def("PrintError", &PrintError);

    py::class_<CCVector3>(m, "Vector3")
            .def(py::init<PointCoordinateType, PointCoordinateType, PointCoordinateType>())
            .def_readwrite("x", &CCVector3::x)
            .def_readwrite("y", &CCVector3::y)
            .def_readwrite("z", &CCVector3::z)
            .def("__mul__", [](const CCVector3 &self, PointCoordinateType val) {
                return self * val;
            })
            .def("__sub__", [](const CCVector3 &self, const CCVector3 &other) {
                return self - other;
            })
            .def("__div__", &CCVector3::operator/)
            .def("__add__", &CCVector3::operator+)

            .def("__repr__", [](const CCVector3 &self) {
                return "<Vector3(" + std::to_string(self.x) + ", " + std::to_string(self.y) + ", " +
                       std::to_string(self.z) + ")>";
            });

    py::class_<ScalarField>(m, "ScalarField")
            .def("getName", &ScalarField::getName)
            .def("setName", &ScalarField::setName)
            .def("asArray", &ScalarField::asArray);

    py::class_<ccPointCloud>(m, "ccPointCloud")
            .def("getName", [](const ccPointCloud *self) { return self->getName().toStdString(); }
            )
            .def("setName", [](ccPointCloud *self, const char *newName) { self->setName(newName); })
            .def("size", &ccPointCloud::size)
            .def("getNumberOfScalarField", &ccPointCloud::getNumberOfScalarFields)
            .def("getScalarField", &ccPointCloud::getScalarField)
            .def("getScalarFieldName", &ccPointCloud::getScalarFieldName)
            .def("getScalarFieldIndexByName", &ccPointCloud::getScalarFieldIndexByName)
            .def("getScalarField", [](const ccPointCloud *self, int index) {
                CCCoreLib::ScalarField *scalarField = self->getScalarField(index);
                if (scalarField == nullptr)
                {
                    throw py::index_error("ScalarField index out of range");
                }
                return ScalarField(scalarField);
            });

    py::class_<ccHObject>(m, "ccHObject")
            .def("toPointCloud", [](ccHObject *obj) {
                bool aya{false};
                return ccHObjectCaster::ToPointCloud(obj, &aya);
            });

}