/*
  Copyright 2007 Albert Strasheim <fullung@gmail.com>

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include "pyactivemq.h"

#include <boost/python/class.hpp>
#include <boost/python/pure_virtual.hpp>

#include <cms/ExceptionListener.h>

namespace py = boost::python;

using cms::ExceptionListener;
using cms::CMSException;

static const char* ExceptionListener_docstring =
    "If a CMS provider detects a serious problem, it notifies the client "
    "application through an C{ExceptionListener} that is registered with "
    "the L{Connection}.";
static const char* ExceptionListener_onException_docstring = 
    "Called when an exception occurs.\n\nOnce notified of an "
    "exception the caller should no longer use the resource "
    "that generated the exception.";

struct ExceptionListenerWrap : ExceptionListener, py::wrapper<ExceptionListener>
{
    virtual void onException(const CMSException& ex)
    {
        PyGILState_STATE gstate = PyGILState_Ensure();
#if 0
        PyObject* obj = py::to_python_indirect<CMSException*, make_owning_holder>()(ex.clone());
#else
        PyObject* obj = NULL;
#endif
        try {
            py::call<void>(this->get_override("onException").ptr(), py::handle<>(obj));
        } catch (const py::error_already_set) {
            // Catch and ignore exception that is thrown if Python
            // onException raised an exception. Print it to sys.stderr,
            // since there is nothing we can do about it here.
            PyErr_Print();
        }
        PyGILState_Release(gstate);
    }
};

void export_ExceptionListener()
{
    py::class_<ExceptionListenerWrap, boost::noncopyable>("ExceptionListener", ExceptionListener_docstring)
        .def("onException", py::pure_virtual(&ExceptionListener::onException), ExceptionListener_onException_docstring)
        ;
}
