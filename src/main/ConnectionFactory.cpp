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

#include <boost/python.hpp>
#include <cms/ConnectionFactory.h>

using namespace boost::python;
using cms::ConnectionFactory;
using cms::Connection;

Connection* (ConnectionFactory::*ConnectionFactory_createConnection0)() =
    &ConnectionFactory::createConnection;
Connection* (ConnectionFactory::*ConnectionFactory_createConnection3)(
    const std::string&, const std::string&, const std::string&) =
    &ConnectionFactory::createConnection;

void export_ConnectionFactory()
{
    class_<ConnectionFactory, boost::noncopyable>("ConnectionFactory", no_init)
        .def("createConnection",
             ConnectionFactory_createConnection0,
             return_value_policy<manage_new_object>())
        .def("createConnection",
             ConnectionFactory_createConnection3,
             return_value_policy<manage_new_object>())
        ;
}
