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

#ifndef PYTHON_PLUGIN_PYTHON_CONFIG_H
#define PYTHON_PLUGIN_PYTHON_CONFIG_H

#include <QString>
#include <QtGlobal>

#include <memory>

#undef slots
#include <pybind11/pybind11.h>

struct PyVenvCfg;
class QProcess;
class PythonConfigPaths;
class QWidget;

/// Simple representation of a SemVer version
struct Version
{
    constexpr Version() = default;

    constexpr Version(uint16_t major_, uint16_t minor_, uint16_t patch_)
        : major(major_), minor(minor_), patch(patch_)
    {
    }

    explicit Version(const QStringRef &versionStr);

    /// Checks whether the Python version number described by
    /// this instance is compatible with the Python version the plugin
    /// was compiled with.
    ///
    /// As explained in https://docs.python.org/3/c-api/stable.html#stable:
    /// CPython’s Application Binary Interface (ABI) is forward- and backwards-compatible
    /// across a minor release.
    /// So, code compiled for Python 3.10.0 will work on 3.10.8 and vice versa,
    /// but will need to be compiled separately for 3.9.x and 3.10.x.
    ///
    /// \return True if the version is compatible.
    bool isCompatibleWithCompiledVersion() const;

    bool isNull() const
    {
        return major == 0 && minor == 0 && patch == 0;
    };

    bool operator==(const Version &other) const;

    uint16_t major{0};
    uint16_t minor{0};
    uint16_t patch{0};
};

/// Python Version the plugin was compiled against
constexpr Version PythonVersion(PY_MAJOR_VERSION, PY_MINOR_VERSION, PY_MICRO_VERSION);

/// This class infers the right python home and python path
/// for the python environment to be used.
///
/// Its only used for Windows (on other platform it doesn't do much) as
/// on Windows we can't rely on the system's python.
class PythonConfig final
{
  public:
    enum class Type
    {
        Venv,
        Conda,
        System,
        Unknown
    };

    /// # On Windows:
    /// Initialize python home and python path
    /// corresponding to the environment to be used.
    ///
    /// # Other Platforms
    /// Does nothing, as we rely on the system's python to be properly installed
    PythonConfig() = default;

    Type type() const
    {
        return m_type;
    }

    template <class ostream> friend ostream &operator<<(ostream &o, Type type)
    {
        switch (type)
        {
        case Type::Venv:
            o << "Venv";
            break;
        case Type::Conda:
            o << "Conda";
            break;
        case Type::System:
            o << "System";
            break;
        case Type::Unknown:
            o << "Unknown";
            break;
        }
        return o;
    }

    /// Sets the necessary settings of the QProcess so that
    /// it uses the correct Python exe.
    void preparePythonProcess(QProcess &pythonProcess) const;

    /// Returns the python home & path stored in
    /// types that the CPython API can use.
    PythonConfigPaths pythonCompatiblePaths() const;

    /// Calls the python.exe of this environment / config
    /// to get its version.
    ///
    /// \return The version returned by the python process
    ///         If the python process failed for whatever reason
    ///         the version will be {0, 0, 0}
    Version getVersion() const;

    /// Does some basic validation (check is python executable exists
    /// and checks if its version is compatible) and displays a GUI with
    /// a message describing the error to the user.
    ///
    /// \param parent parent for the GUI to be displayed, can be nullptr
    /// \return true if the config passes the validation
    ///         (meaning no error where displayed to the user)
    bool validateAndDisplayErrors(QWidget *parent = nullptr) const;

    static bool IsInsideEnvironment();
    static PythonConfig fromContainingEnvironment();

    void initDefault();
#ifdef Q_OS_WIN32
    /// Initialize the paths to point to where the Python
    /// environment was bundled on a Windows installation
    void initBundled();
#endif
    /// Initialize from the path to an environment.
    /// Will try to guess if the environment is a conda env
    /// or a python venv
    void initFromLocation(const QString &prefix);
    /// Initialize the paths to use the conda environment stored at condaPrefix
    void initCondaEnv(const QString &condaPrefix);
    /// Initialize the paths to use the python venv stored at venvPrefix.
    void initVenv(const QString &venvPrefix);

    void initFromPythonExecutable(const QString &pythonExecutable);

    template <class ostream> friend ostream &operator<<(ostream &o, const PythonConfig &config)
    {
        o << "PythonConfig { type: " << config.m_type << ", home: '" << config.m_pythonHome
          << "', path: '" << config.m_pythonPath << "'}";
        return o;
    }

  private:
    QString m_pythonHome{};
    QString m_pythonPath{};
    Type m_type{Type::Unknown};
};

/// Holds strings of the PythonHome & PythonPath,
/// in types that are compatible with CPython API.
///
/// They are meant to be used for `Py_SetPythonHome` and `Py_SetPath`.
/// See:
///  - https://docs.python.org/3/c-api/init.html#c.Py_SetPythonHome
///  - https://docs.python.org/3/c-api/init.html#c.Py_SetPath
class PythonConfigPaths final
{
    friend PythonConfig;

  public:
    /// Default ctor, does not initialize pythonHome & pythonPath
    PythonConfigPaths() = default;

    /// returns true if both paths are non empty
    bool isSet() const;

    /// Returns the pythonHome
    const wchar_t *pythonHome() const;

    /// Returns the pythonPath
    const wchar_t *pythonPath() const;

  private:
    /// Once Py_SetPythonHome is used, the value of m_pythonHome must never change
    /// and must not be freed until the interpreter is uninitialized.
    std::unique_ptr<wchar_t[]> m_pythonHome{};
    /// m_pythonPath can however be freed after Py_SetPath was used
    std::unique_ptr<wchar_t[]> m_pythonPath{};
};
#endif // PYTHON_PLUGIN_PYTHON_CONFIG_H
