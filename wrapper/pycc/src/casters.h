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

#ifndef PYTHON_PLUGIN_CASTERS_H
#define PYTHON_PLUGIN_CASTERS_H

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
            std::runtime_error("Unable to extract bytes contents!");
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
        switch (src.type())
        {
        case QVariant::Invalid:
            break;
        case QVariant::Bool:
            h = pybind11::cast(src.toBool(), policy, handle);
            break;
        case QVariant::Int:
            h = pybind11::cast(src.toInt(), policy, handle);
            break;
        case QVariant::UInt:
            h = pybind11::cast(src.toUInt(), policy, handle);
            break;
        case QVariant::LongLong:
            h = pybind11::cast(src.toLongLong(), policy, handle);
            break;
        case QVariant::ULongLong:
            h = pybind11::cast(src.toULongLong(), policy, handle);
            break;
        case QVariant::Double:
            h = pybind11::cast(src.toDouble(), policy, handle);
            break;
        case QVariant::Char:
            h = pybind11::cast(src.toChar(), policy, handle);
            break;
        case QVariant::String:
            h = pybind11::cast(src.toString(), policy, handle);
            break;
        case QVariant::Map:
            Q_FALLTHROUGH();
        case QVariant::List:
            Q_FALLTHROUGH();
        case QVariant::StringList:
            Q_FALLTHROUGH();
        case QVariant::ByteArray:
            h = pybind11::cast(src.toByteArray(), policy, handle);
            break;
        case QVariant::BitArray:
            Q_FALLTHROUGH();
        case QVariant::Date:
            Q_FALLTHROUGH();
        case QVariant::Time:
            Q_FALLTHROUGH();
        case QVariant::DateTime:
            Q_FALLTHROUGH();
        case QVariant::Url:
            Q_FALLTHROUGH();
        case QVariant::Locale:
            Q_FALLTHROUGH();
        case QVariant::Rect:
            Q_FALLTHROUGH();
        case QVariant::RectF:
            Q_FALLTHROUGH();
        case QVariant::Size:
            Q_FALLTHROUGH();
        case QVariant::SizeF:
            Q_FALLTHROUGH();
        case QVariant::Line:
            Q_FALLTHROUGH();
        case QVariant::LineF:
            Q_FALLTHROUGH();
        case QVariant::Point:
            Q_FALLTHROUGH();
        case QVariant::PointF:
            Q_FALLTHROUGH();
        case QVariant::RegExp:
            Q_FALLTHROUGH();
        case QVariant::RegularExpression:
            Q_FALLTHROUGH();
        case QVariant::Hash:
            Q_FALLTHROUGH();
        case QVariant::EasingCurve:
            Q_FALLTHROUGH();
        case QVariant::Uuid:
            Q_FALLTHROUGH();
        case QVariant::ModelIndex:
            Q_FALLTHROUGH();
        case QVariant::PersistentModelIndex:
            Q_FALLTHROUGH();
        case QVariant::LastCoreType:
            Q_FALLTHROUGH();
        case QVariant::Font:
            Q_FALLTHROUGH();
        case QVariant::Pixmap:
            Q_FALLTHROUGH();
        case QVariant::Brush:
            Q_FALLTHROUGH();
        case QVariant::Color:
            Q_FALLTHROUGH();
        case QVariant::Palette:
            Q_FALLTHROUGH();
        case QVariant::Image:
            Q_FALLTHROUGH();
        case QVariant::Polygon:
            Q_FALLTHROUGH();
        case QVariant::Region:
            Q_FALLTHROUGH();
        case QVariant::Bitmap:
            Q_FALLTHROUGH();
        case QVariant::Cursor:
            Q_FALLTHROUGH();
        case QVariant::KeySequence:
            Q_FALLTHROUGH();
        case QVariant::Pen:
            Q_FALLTHROUGH();
        case QVariant::TextLength:
            Q_FALLTHROUGH();
        case QVariant::TextFormat:
            Q_FALLTHROUGH();
        case QVariant::Matrix:
            Q_FALLTHROUGH();
        case QVariant::Transform:
            Q_FALLTHROUGH();
        case QVariant::Matrix4x4:
            Q_FALLTHROUGH();
        case QVariant::Vector2D:
            Q_FALLTHROUGH();
        case QVariant::Vector3D:
            Q_FALLTHROUGH();
        case QVariant::Vector4D:
            Q_FALLTHROUGH();
        case QVariant::Quaternion:
            Q_FALLTHROUGH();
        case QVariant::Icon:
            Q_FALLTHROUGH();
        case QVariant::LastGuiType:
            Q_FALLTHROUGH();
        case QVariant::SizePolicy:
            Q_FALLTHROUGH();
        case QVariant::UserType:
            Q_FALLTHROUGH();
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        case QVariant::PolygonF:
            Q_FALLTHROUGH();
#endif
        case QVariant::LastType:
            throw std::runtime_error("Cannot convert this QVariant to python object");
        }
        return h.release();
    }
};

} // namespace detail
} // namespace pybind11
#endif // PYTHON_PLUGIN_CASTERS_H
