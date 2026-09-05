#pragma once
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

#include <QSharedPointer>
#include <QString>
#include <QVariant>

#include <ccColorScale.h>
#include <ccLog.h>

// clang-format off
#undef slots
#include <pybind11/pybind11.h>
#include <Python.h>
// clang-format on

PYBIND11_DECLARE_HOLDER_TYPE(T, QSharedPointer<T>);
PYBIND11_MAKE_OPAQUE(QSharedPointer<ccColorScale>);

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

    static handle cast(const QString &src, return_value_policy policy, handle parent)
    {
        const QByteArray byteArray = src.toUtf8();
        // TODO find how to use this error parameter.
        const char *errors = nullptr;
        PyObject *obj = PyUnicode_DecodeUTF8(byteArray.constData(), byteArray.size(), errors);
        return obj;
    }
};

template <> struct type_caster<QByteArray>
{
    PYBIND11_TYPE_CASTER(QByteArray, _("QByteArray"));

    bool load(handle src, bool)
    {
        if (!src || !isinstance<pybind11::bytes>(src))
        {
            return false;
        }

        char *buffer;
        ssize_t length;
        if (PyBytes_AsStringAndSize(src.ptr(), &buffer, &length) == -1)
        {
            throw std::runtime_error("Unable to extract bytes contents!");
        }
        value = QByteArray(buffer, length);
        return true;
    }

    static handle cast(const QByteArray &src, return_value_policy /* policy */, handle /* handle */)
    {
        return PyBytes_FromStringAndSize(src.constData(), src.size());
    }
};

template <> struct type_caster<QVariant>
{
    PYBIND11_TYPE_CASTER(QVariant, _("QVariant"));

    bool load(handle src, bool)
    {
        if (!src)
        {
            return false;
        }
        if (src.is_none())
        {
            return true;
        }

        if (isinstance<pybind11::int_>(src))
        {
            value = src.cast<int>();
        }
        else if (isinstance<pybind11::str>(src))
        {
            value = QString::fromStdString(src.cast<std::string>());
        }
        else if (isinstance<pybind11::float_>(src))
        {
            value = src.cast<double>();
        }
        else
        {
            return false;
        }
        return true;
    }

    static handle cast(const QVariant &src, return_value_policy policy, handle handle)
    {
        pybind11::object h = none();
        switch (src.metaType().id())
        {
        case QMetaType::Void:
            break;
        case QMetaType::Bool:
            h = pybind11::cast(src.toBool(), policy, handle);
            break;
        case QMetaType::Int:
            h = pybind11::cast(src.toInt(), policy, handle);
            break;
        case QMetaType::UInt:
            h = pybind11::cast(src.toUInt(), policy, handle);
            break;
        case QMetaType::LongLong:
            h = pybind11::cast(src.toLongLong(), policy, handle);
            break;
        case QMetaType::ULongLong:
            h = pybind11::cast(src.toULongLong(), policy, handle);
            break;
        case QMetaType::Double:
            h = pybind11::cast(src.toDouble(), policy, handle);
            break;
        case QMetaType::Char:
            h = pybind11::cast(src.toChar(), policy, handle);
            break;
        case QMetaType::QString:
            h = pybind11::cast(src.toString(), policy, handle);
            break;
        case QMetaType::QByteArray:
            h = pybind11::cast(src.toByteArray(), policy, handle);
            break;
        default:
            throw std::runtime_error("Cannot convert this QMetaType to python object");
        }
        return h.release();
    }
};

} // namespace detail
} // namespace pybind11
