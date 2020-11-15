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
//#                   COPYRIGHT: Thomas Montaigu                           #
//#                                                                        #
//##########################################################################

#include "ccGUIPythonInstance.h"


#define slots Q_SLOTS
#define signals Q_SIGNALS
#include <ccPointCloud.h>

#include <stdexcept>

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
    case CC_FERR_NOT_IMPLEMENTED:
        throw std::runtime_error("Not implemented");
    }
}

ccGUIPythonInstance::ccGUIPythonInstance(ccMainAppInterface *app) : m_app(app)
{
    if (m_app == nullptr)
    {
        throw std::invalid_argument("nullptr received for the app");
    }
}

ccHObject *ccGUIPythonInstance::loadFile(const char *filename, FileIOFilter::LoadParameters &parameters)
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

ccHObject *ccGUIPythonInstance::createObject(const char *type_name)
{
    if (strcmp(type_name, "ccPointCloud") == 0)
    {
        auto obj = new ccPointCloud();
        m_pythonDB.push_back(obj);
        return obj;
    }
    else
    {
        throw std::invalid_argument("Unknown type");
    }
}

void ccGUIPythonInstance::addToDB(
    ccHObject *obj, bool updateZoom, bool autoExpandDBTree, bool checkDimensions, bool autoRedraw)
{
    auto pos = std::find(std::begin(m_pythonDB), std::end(m_pythonDB), obj);

    if (pos != std::end(m_pythonDB))
    {
        m_pythonDB.erase(pos);
    }

    m_app->addToDB(obj, updateZoom, autoExpandDBTree, checkDimensions, autoRedraw);
}

size_t ccGUIPythonInstance::clearDB()
{
    size_t n = m_pythonDB.size();
    for (ccHObject *obj : m_pythonDB)
    {
        delete obj;
    }
    m_pythonDB.clear();
    return n;
}
