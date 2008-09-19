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

#include <boost/python/scope.hpp>
#include <boost/python/class.hpp>

#include <activemq/core/ActiveMQConnectionFactory.h>

namespace py = boost::python;

using activemq::core::ActiveMQConnectionFactory;
using cms::ConnectionFactory;

static const char* AMQConnectionFactory_docstring =
    "Interface for a connection with the ActiveMQ message broker.";
static const char* AMQConnectionFactory_username_docstring =
    "Username to authenticate with.";
static const char* AMQConnectionFactory_password_docstring =
    "Password to authenticate with.";
static const char* AMQConnectionFactory_brokerURL_docstring =
    "The URL of the broker we are connecting to.";

void export_ActiveMQConnectionFactory()
{
    py::class_<ActiveMQConnectionFactory, py::bases<ConnectionFactory>, boost::noncopyable>("ActiveMQConnectionFactory", AMQConnectionFactory_docstring)
        .def(py::init<const std::string&, py::optional<const std::string&, const std::string&> >())
        .add_property("username",
                      make_function(&ActiveMQConnectionFactory::getUsername,
                                    py::return_value_policy<py::return_by_value>()),
                      &ActiveMQConnectionFactory::setUsername,
                      AMQConnectionFactory_username_docstring)
        .add_property("password",
                      make_function(&ActiveMQConnectionFactory::getPassword,
                                    py::return_value_policy<py::return_by_value>()),
                      &ActiveMQConnectionFactory::setPassword,
                      AMQConnectionFactory_password_docstring)
        .add_property("brokerURL",
                      make_function(&ActiveMQConnectionFactory::getBrokerURL,
                                    py::return_value_policy<py::return_by_value>()),
                      &ActiveMQConnectionFactory::setBrokerURL,
                      AMQConnectionFactory_brokerURL_docstring)
        ;
}
