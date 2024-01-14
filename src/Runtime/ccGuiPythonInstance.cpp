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

#include "ccGuiPythonInstance.h"

#define slots Q_SLOTS
#define signals Q_SIGNALS

#include <ccGLWindowInterface.h>
#include <ccMainAppInterface.h>
#include <ccPointCloud.h>

#include <QMainWindow>

#include <stdexcept>

#undef slots
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "../../wrapper/pycc/src/casters.h"

namespace py = pybind11;
using namespace pybind11::literals;

static void ThrowForFileError(CC_FILE_ERROR error)
{
    switch (error)
    {
    case CC_FERR_NO_ERROR:
        break;
    case CC_FERR_BAD_ARGUMENT:
        throw std::runtime_error("Bad argument");
    case CC_FERR_UNKNOWN_FILE:
        throw std::runtime_error("Unknown file");
    case CC_FERR_WRONG_FILE_TYPE:
        throw std::runtime_error("Wrong file type");
    case CC_FERR_WRITING:
        throw std::runtime_error("Error when writing");
    case CC_FERR_READING:
        throw std::runtime_error("Error when reading");
    case CC_FERR_NO_SAVE:
        throw std::runtime_error("Nothing to save");
    case CC_FERR_NO_LOAD:
        throw std::runtime_error("Nothing to load");
    case CC_FERR_BAD_ENTITY_TYPE:
        throw std::runtime_error("Bad entity type");
    case CC_FERR_CANCELED_BY_USER:
        throw std::runtime_error("Canceled by user");
    case CC_FERR_NOT_ENOUGH_MEMORY:
        throw std::runtime_error("Not enough memory");
    case CC_FERR_MALFORMED_FILE:
        throw std::runtime_error("Malformed File");
    case CC_FERR_CONSOLE_ERROR:
        throw std::runtime_error("The error has been logged in the console");
    case CC_FERR_BROKEN_DEPENDENCY_ERROR:
        throw std::runtime_error("Broken dependency");
    case CC_FERR_FILE_WAS_WRITTEN_BY_UNKNOWN_PLUGIN:
        throw std::runtime_error("File was written by unknown plugin");
    case CC_FERR_THIRD_PARTY_LIB_FAILURE:
        throw std::runtime_error("Third party lib failure");
    case CC_FERR_THIRD_PARTY_LIB_EXCEPTION:
        throw std::runtime_error("Third party lib exception");
    case CC_FERR_INTERNAL:
        throw std::runtime_error("Internal error");
    case CC_FERR_NOT_IMPLEMENTED:
        throw std::runtime_error("Not implemented");
    }
}

ccGuiPythonInstance::ccGuiPythonInstance(ccMainAppInterface *app) noexcept(false) : m_app(app)
{
    if (m_app == nullptr)
    {
        throw std::invalid_argument("nullptr received for the app");
    }
}

ccGLWindowInterface *ccGuiPythonInstance::getActiveGLWindow()
{
    return m_app->getActiveGLWindow();
}

QMainWindow *ccGuiPythonInstance::getMainWindow()
{
    return m_app->getMainWindow();
}

bool ccGuiPythonInstance::haveSelection() const
{
    return m_app->haveSelection();
}

bool ccGuiPythonInstance::haveOneSelection() const
{
    return m_app->haveOneSelection();
}

const ccHObject::Container &ccGuiPythonInstance::getSelectedEntities() const
{
    return m_app->getSelectedEntities();
}

void ccGuiPythonInstance::setSelectedInDB(ccHObject *obj, const bool selected)
{
    m_app->setSelectedInDB(obj, selected);
}

ccHObject *ccGuiPythonInstance::loadFile(const char *filename,
                                         FileIOFilter::LoadParameters &parameters)
{
    CC_FILE_ERROR result = CC_FERR_NO_ERROR;
    ccHObject *newGroup = FileIOFilter::LoadFromFile(filename, parameters, result);
    ThrowForFileError(result);

    if (newGroup)
    {
        m_app->addToDB(newGroup);
        m_app->refreshAll();
        m_app->updateUI();
    }
    return newGroup;
}

void ccGuiPythonInstance::addToDB(pybind11::object &obj,
                                  bool updateZoom,
                                  bool autoExpandDBTree,
                                  bool checkDimensions,
                                  bool autoRedraw)
{
    try
    {
        auto *const hobj = obj.cast<ccHObject *>();
        m_app->addToDB(hobj, updateZoom, autoExpandDBTree, checkDimensions, autoRedraw);
        obj.inc_ref();
    }
    catch (const pybind11::cast_error &)
    {
        throw std::runtime_error("Cannot add to the DB a type that does not sub class ccHObject");
    }
}

void ccGuiPythonInstance::removeFromDB(pybind11::object &obj)
{
    try
    {
        auto *const hobj = obj.cast<ccHObject *>();
        // Python side will take care of deleting it
        // because if we do delete it now and user tries to use the obj
        // it'll be a use after free.
        m_app->removeFromDB(hobj, false /*delete*/);
        Q_ASSERT(obj.ref_count() >= 2);
        obj.dec_ref();
    }
    catch (const pybind11::cast_error &)
    {
        throw std::runtime_error(
            "Cannot remove from the DB a type that does not sub class ccHObject");
    }
}

ccHObject *ccGuiPythonInstance::dbRootObject()
{
    return m_app->dbRootObject();
}

void ccGuiPythonInstance::redrawAll(bool only2D)
{
    m_app->redrawAll(only2D);
}

void ccGuiPythonInstance::refreshAll(bool only2D)
{
    m_app->refreshAll(only2D);
}

void ccGuiPythonInstance::enableAll()
{
    m_app->enableAll();
}

void ccGuiPythonInstance::disableAll()
{
    m_app->disableAll();
}

void ccGuiPythonInstance::updateUI()
{
    m_app->updateUI();
}

void ccGuiPythonInstance::freezeUI(bool state)
{
    m_app->freezeUI(state);
}
ccMainAppInterface *ccGuiPythonInstance::app()
{
    return m_app;
}

void define_ccGUIPythonInstance(py::module &m)
{
    py::class_<ccGuiPythonInstance>(m,
                                    "ccPythonInstance",
                                    R"(
    Class to interact with the running GUI of CloudCompare.

    The methods allow you to get/add objects to GUI DB Tree.
    Refresh/freeze/update the UI and display.
    Get selected objects, deselect/set selected state for entities...
)")
        .def("getMainWindow",
             &ccGuiPythonInstance::getMainWindow,
             py::return_value_policy::reference)
        .def("getActiveGLWindow",
             &ccGuiPythonInstance::getActiveGLWindow,
             py::return_value_policy::reference)
        .def("haveSelection",
             &ccGuiPythonInstance::haveSelection,
             R"(Returns true if at least one entity is selected in the GUI DB Tree)")
        .def("haveOneSelection",
             &ccGuiPythonInstance::haveOneSelection,
             R"(Returns True if exactly one entity is selected in the GUI DB Tree)")
        .def("getSelectedEntities",
             &ccGuiPythonInstance::getSelectedEntities,
             py::return_value_policy::reference,
             R"(Returns the list of selected entities in the GUI DB Tree)")
        .def("setSelectedInDB",
             &ccGuiPythonInstance::setSelectedInDB,
             "obj"_a,
             "selected"_a,
             R"(Set the selection state of the object in the GUI DB Tree)")
        .def("dbRootObject",
             &ccGuiPythonInstance::dbRootObject,
             py::return_value_policy::reference,
             R"(Returns the root of the GUI DB Tree)")
        .def("addToDB",
             &ccGuiPythonInstance::addToDB,
             "obj"_a,
             "updateZoom"_a = false,
             "autoExpandDBTree"_a = true,
             "checkDimensions"_a = false,
             "autoRedraw"_a = true,
             R"(Adds the object to the GUI DB Tree)")
        .def("removeFromDB", &ccGuiPythonInstance::removeFromDB, "entity"_a)
        .def("redrawAll", &ccGuiPythonInstance::redrawAll, "only2D"_a = false)
        .def("refreshAll", &ccGuiPythonInstance::refreshAll, "only2D"_a = false)
        .def("enableAll", &ccGuiPythonInstance::enableAll)
        .def("disableAll", &ccGuiPythonInstance::disableAll)
        .def("updateUI", &ccGuiPythonInstance::updateUI)
        .def("freezeUI", &ccGuiPythonInstance::freezeUI, "state"_a)
        .def("app", &ccGuiPythonInstance::app)
        .def("loadFile",
             &ccGuiPythonInstance::loadFile,
             py::return_value_policy::reference,
             "filepath"_a,
             "loadParameters"_a,
             R"(
    Loads the file located at the filepath.

    Adds the file content of the file into the DBTree
    and then returns loaded object handle.

    raises RuntimeError in case of error
                                                                                 )");
}
