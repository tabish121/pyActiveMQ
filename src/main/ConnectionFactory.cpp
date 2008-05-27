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
#include <boost/python/manage_new_object.hpp>

#include <cms/ConnectionFactory.h>

namespace py = boost::python;

using cms::ConnectionFactory;
using cms::Connection;

static const char* ConnectionFactory_docstring = "Defines the interface for a factory that creates connection objects.";

void export_ConnectionFactory()
{
    Connection* (ConnectionFactory::*ConnectionFactory_createConnection0)() =
        &ConnectionFactory::createConnection;
    Connection* (ConnectionFactory::*ConnectionFactory_createConnection2)(
        const std::string&, const std::string&) =
        &ConnectionFactory::createConnection;
    Connection* (ConnectionFactory::*ConnectionFactory_createConnection3)(
        const std::string&, const std::string&, const std::string&) =
        &ConnectionFactory::createConnection;

    py::class_<ConnectionFactory, boost::noncopyable>("ConnectionFactory", ConnectionFactory_docstring, py::no_init)
        .def("createConnection",
             ConnectionFactory_createConnection0,
             py::return_value_policy<py::manage_new_object>())
        .def("createConnection",
             ConnectionFactory_createConnection2,
             py::return_value_policy<py::manage_new_object>(),
             (py::arg("username"), py::arg("password")))
        .def("createConnection",
             ConnectionFactory_createConnection3,
             py::return_value_policy<py::manage_new_object>(),
             (py::arg("username"), py::arg("password"), py::arg("clientID")))
        ;
}
