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

#ifndef CLOUDCOMPAREPROJECTS_CASTERS_H
#define CLOUDCOMPAREPROJECTS_CASTERS_H

#include <QString>

#undef slots
#include <pybind11/pybind11.h>
#include <Python.h>

namespace pybind11
{
namespace detail
{
template <> struct type_caster<QString>
{
    PYBIND11_TYPE_CASTER(QString, _("QString"));

    bool load(handle src, bool)
    {
        PyObject *source = src.ptr();

        const char *str = PyUnicode_AsUTF8(source);
        if (!str)
        {
            return false;
        }

        value = str;
        return true;
    }

    static handle cast(const QString &src, return_value_policy /* policy */, handle /* parent */)
    {
        // TODO Is there a way to avoid the extra conversion to stdString ?
        return PyUnicode_FromString(src.toStdString().c_str());
    }
};
} // namespace detail
} // namespace pybind11
#endif // CLOUDCOMPAREPROJECTS_CASTERS_H
