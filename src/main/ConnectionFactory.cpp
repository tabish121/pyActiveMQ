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

#include <cms/Connection.h>
#include <cms/ConnectionFactory.h>

namespace py = boost::python;

using cms::ConnectionFactory;
using cms::Connection;

static const char* ConnectionFactory_docstring =
    "Defines the interface for a factory that creates connection objects.";
static const char* ConnectionFactory_createConnection0_docstring = 
    "Creates a connection with the default user identity.\n\n"
    "The connection is created in stopped mode. No messages "
    "will be delivered until the L{Connection.start} method "
    "is explicitly called.";
static const char* ConnectionFactory_createConnection2_docstring =
    "Creates a connection with the default specified identity."
    "\n\nThe connection is created in stopped mode. No "
    "messages will be delivered until the L{Connection.start} "
    "method is explicitly called. The C{username} and "
    "C{password} values passed here do not change the "
    "defaults, subsequent calls to the parameterless "
    "C{createConnection} will continue to use the default "
    "values that were set in the constructor.";
static const char* ConnectionFactory_createConnection3_docstring =
    "Creates a connection with the specified user identity."
    "\n\nThe connection is created in stopped mode. No "
    "messages will be delivered until the L{Connection.start} "
    "method is explicitly called. The C{username} and "
    "C{password} values passed here do not change the "
    "defaults, subsequent calls to the parameterless "
    "C{createConnection} will continue to use the default "
    "values that were set in the constructor.";

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
             py::return_value_policy<py::manage_new_object>(),
             ConnectionFactory_createConnection0_docstring)
        .def("createConnection",
             ConnectionFactory_createConnection2,
             py::return_value_policy<py::manage_new_object>(),
             (py::arg("username"), py::arg("password")),
             ConnectionFactory_createConnection2_docstring)
        .def("createConnection",
             ConnectionFactory_createConnection3,
             py::return_value_policy<py::manage_new_object>(),
             (py::arg("username"), py::arg("password"), py::arg("clientID")),
             ConnectionFactory_createConnection3_docstring)
        ;
}
