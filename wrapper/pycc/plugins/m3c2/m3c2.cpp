#include <pybind11/pybind11.h>

#include <qM3C2Dialog.h>
#include <qM3C2Process.h>

#include <ccMainAppInterface.h>
#include <ccPointCloud.h>

#include <stdexcept>

#include "../../src/casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

ccPointCloud *ComputeWrapper(const qM3C2Dialog &dlg, bool allowDialogs)
{
    QString errorMsg;
    ccPointCloud *outputCloud;

    // These should be arguments but pybind11 trips on them
    // in standalone mode, with a TypeError, not sure why
    QWidget *parentWidget = nullptr;
    ccMainAppInterface *app = nullptr;
    if (app != nullptr)
    {
        // It makes CC crash
        throw py::value_error("non null app pointer is not supported");
    }

    bool success =
        qM3C2Process::Compute(dlg, errorMsg, outputCloud, allowDialogs, parentWidget, app);
    if (!success)
    {
        const std::string msg = errorMsg.toStdString();
        throw std::runtime_error(msg.c_str());
    }
    return outputCloud;
}

void define_m3c2_plugin(py::module_ &m)
{
    m.attr("plugin_name") = "qM3C2";

    py::class_<qM3C2Dialog> pyM3C2Dialog(m, "qM3C2Dialog");

    py::enum_<qM3C2Dialog::ExportOptions>(pyM3C2Dialog, "ExportOptions")
        .value("PROJECT_ON_CLOUD1", qM3C2Dialog::ExportOptions::PROJECT_ON_CLOUD1)
        .value("PROJECT_ON_CLOUD2", qM3C2Dialog::ExportOptions::PROJECT_ON_CLOUD2)
        .value("PROJECT_ON_CORE_POINTS", qM3C2Dialog::ExportOptions::PROJECT_ON_CORE_POINTS)
        .export_values();

    pyM3C2Dialog
        .def(py::init([](ccPointCloud *cloud1, ccPointCloud *cloud2)
                      { return new qM3C2Dialog(cloud1, cloud2, nullptr); }),
             "cloud1"_a,
             "cloud2"_a,
             py::return_value_policy::take_ownership)
        .def("getCloud1", &qM3C2Dialog::getCloud1, py::return_value_policy::reference)
        .def("getCloud2", &qM3C2Dialog::getCloud2, py::return_value_policy::reference)
        .def("getCorePointsCloud",
             &qM3C2Dialog::getCorePointsCloud,
             py::return_value_policy::reference)
        .def("setCorePointsCloud", &qM3C2Dialog::setCorePointsCloud, "cloud"_a)
        .def("getNormalsOrientationCloud",
             &qM3C2Dialog::getNormalsOrientationCloud,
             py::return_value_policy::reference)
        .def("getExportOption", &qM3C2Dialog::getExportOption)
        .def("keepOriginalCloud", &qM3C2Dialog::keepOriginalCloud)
        .def("getMaxThreadCount", &qM3C2Dialog::getMaxThreadCount)
        .def("loadParamsFromFile", &qM3C2Dialog::loadParamsFromFile, "filename"_a)
        .def("loadParamsFromPersistentSettings", &qM3C2Dialog::loadParamsFromPersistentSettings)
        .def("saveParamsToPersistentSettings", &qM3C2Dialog::saveParamsToPersistentSettings)
        .def("exec", &qM3C2Dialog::exec);

    py::class_<qM3C2Process>(m, "qM3C2Process")
        .def_static("Compute", &ComputeWrapper, "dialog"_a, "allowsDialog"_a);
}

PYBIND11_MODULE(pym3c2, m)
{
    define_m3c2_plugin(m);
}
