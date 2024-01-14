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

#ifndef PYTHON_PLUGIN_THREADING_H
#define PYTHON_PLUGIN_THREADING_H

#include <QException>

#include <pybind11/pybind11.h>

namespace py = pybind11;

class PyThreadStateGuard
{
  public:
    explicit PyThreadStateGuard(PyInterpreterState *interpState)
    {
        pCurrentState = PyThreadState_New(interpState);
        PyEval_AcquireThread(pCurrentState);
    }

    virtual ~PyThreadStateGuard()
    {
        PyThreadState_Clear(pCurrentState);
        PyEval_ReleaseThread(pCurrentState);

        PyThreadState_Delete(pCurrentState);
    }

  private:
    PyThreadState *pCurrentState{nullptr};
};

struct PyThreadStateReleaser
{
    explicit PyThreadStateReleaser() : state(PyEval_SaveThread()) {}

    virtual ~PyThreadStateReleaser()
    {
        PyEval_RestoreThread(state);
    }

    PyThreadState *state{nullptr};
};

class ThreadException : public QException
{
  public:
    explicit ThreadException(const std::exception &err) : e(err) {}

    void raise() const override
    {
        throw *this;
    }

    QException *clone() const override
    {
        return new ThreadException(*this);
    }

    const char *what() const noexcept override
    {
        return e.what();
    }

    std::exception error() const
    {
        return e;
    }

  private:
    std::exception e;
};

inline py::object
call_fn(PyThreadState *main_state, py::object callable, py::args args, py::kwargs kwargs)
{
    PyThreadStateGuard threadStateGuard{main_state->interp};
    try
    {
        return callable(*args, **kwargs);
    }
    catch (const std::exception &e)
    {
        throw ThreadException(e);
    }
}
#endif // PYTHON_PLUGIN_THREADING_H
