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

#include <boost/python/class.hpp>
#include <boost/python/scope.hpp>
#include <boost/python/exception_translator.hpp>

#include <cms/CMSSecurityException.h>

namespace py = boost::python;

using cms::CMSSecurityException;

static PyObject* pyex = PyErr_NewException("pyactivemq.CMSSecurityException", PyExc_Exception, NULL);

void CMSSecurityException_translator(const CMSSecurityException& e)
{
    PyObject* message = PyString_FromString(e.getMessage().c_str());
    PyObject_SetAttrString(pyex, "message", message);
    PyErr_SetString(pyex, e.getStackTraceString().c_str());
}

void export_CMSSecurityException()
{
    PyModule_AddObject(py::detail::current_scope, "CMSSecurityException", pyex);
    py::register_exception_translator<CMSSecurityException>(CMSSecurityException_translator);
}
