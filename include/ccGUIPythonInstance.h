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

#ifndef CLOUDCOMPAREPROJECTS_CCGUIPYTHONINSTANCE_H
#define CLOUDCOMPAREPROJECTS_CCGUIPYTHONINSTANCE_H

#include <FileIOFilter.h>

class ccMainAppInterface;
class QMainWindow;
class ccGLWindow;

namespace pybind11
{
class args;
class kwargs;
class object;
} // namespace pybind11

/// This class provide methods that are made available Python scripts
/// kind of like C++ plugins have access to a `ccMainAppInterface`.
/// Thus this class is mostly a ccMainAppInterface with some accommodations
/// to handle the fact that it is made to interact with python.
class Q_DECL_EXPORT ccGUIPythonInstance
{
  public:
    explicit ccGUIPythonInstance(ccMainAppInterface *app);

    QMainWindow *getMainWindow();

    ccGLWindow *getActiveGLWindow();

    bool haveSelection() const;

    bool haveOneSelection() const;

    const ccHObject::Container &getSelectedEntities() const;

    void setSelectedInDB(ccHObject *obj, bool selected);

    ccHObject *dbRootObject();

    void addToDB(ccHObject *obj,
                 bool updateZoom = false,
                 bool autoExpandDBTree = true,
                 bool checkDimensions = false,
                 bool autoRedraw = true);

    void redrawAll(bool only2D = false);

    void refreshAll(bool only2D = false);

    void enableAll();

    void disableAll();

    void updateUI();

    void freezeUI(bool state);

    ccHObject *
    createObject(const char *type_name, const pybind11::args &args, const pybind11::kwargs &kwargs);

    ccHObject *
    createObject(const pybind11::object& class_, const pybind11::args &args, const pybind11::kwargs &kwargs);

    ccHObject *loadFile(const char *filename, FileIOFilter::LoadParameters &parameters);

    /// Clears the internal database of objects that were allocated on the python side
    /// but were not added to the DB tree via `addToDB`
    size_t clearDB();

  private:
    ccMainAppInterface *m_app;
    // This holds pointer to objects that were created by a Python script.
    // They are kept in this vector until they are either moved to the true ccDB (using `addToDB`)
    // or when `clearDB` is called (which is a method not exposed to the python side
    // and the c++ takes care of calling the `clearDB` fn when appropriate).
    std::vector<ccHObject *> m_pythonDB;
};

#endif // CLOUDCOMPAREPROJECTS_CCGUIPYTHONINSTANCE_H
