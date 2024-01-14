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

#ifndef PYTHON_PLUGIN_CC_GUI_PYTHON_INSTANCE_H
#define PYTHON_PLUGIN_CC_GUI_PYTHON_INSTANCE_H

#include <FileIOFilter.h>

class ccMainAppInterface;
class QMainWindow;
class ccGLWindowInterface;

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
class Q_DECL_EXPORT ccGuiPythonInstance final
{
  public:
    explicit ccGuiPythonInstance(ccMainAppInterface *app) noexcept(false);

    QMainWindow *getMainWindow();

    ccGLWindowInterface *getActiveGLWindow();

    bool haveSelection() const;

    bool haveOneSelection() const;

    const ccHObject::Container &getSelectedEntities() const;

    void setSelectedInDB(ccHObject *obj, bool selected);

    ccHObject *dbRootObject();

    void addToDB(pybind11::object &obj,
                 bool updateZoom = false,
                 bool autoExpandDBTree = true,
                 bool checkDimensions = false,
                 bool autoRedraw = true);

    void removeFromDB(pybind11::object &obj);

    void redrawAll(bool only2D = false);

    void refreshAll(bool only2D = false);

    void enableAll();

    void disableAll();

    void updateUI();

    void freezeUI(bool state);

    ccHObject *loadFile(const char *filename, FileIOFilter::LoadParameters &parameters);

    ccMainAppInterface *app();

  private:
    ccMainAppInterface *m_app;
};

#endif // PYTHON_PLUGIN_CC_GUI_PYTHON_INSTANCE_H
