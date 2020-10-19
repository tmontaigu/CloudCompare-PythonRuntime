//
// Created by Thomas on 09/06/2020.
//

#ifndef CLOUDCOMPAREPROJECTS_CASTERS_H
#define CLOUDCOMPAREPROJECTS_CASTERS_H

#include <Python.h>
#include <QString>
#include <pybind11/pybind11.h>

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
