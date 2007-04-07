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
#include <activemq/core/ActiveMQConnectionFactory.h>

using namespace boost::python;
using activemq::core::ActiveMQConnectionFactory;
using cms::ConnectionFactory;

void export_ActiveMQConnectionFactory()
{
    class_<ActiveMQConnectionFactory, bases<ConnectionFactory>, boost::noncopyable>("ActiveMQConnectionFactory")
        .def(init<const std::string&, optional<const std::string&, const std::string&> >())
        .add_property("username",
                      make_function(&ActiveMQConnectionFactory::getUsername,
                                    return_value_policy<return_by_value>()),
                      &ActiveMQConnectionFactory::setUsername)
        .add_property("password",
                      make_function(&ActiveMQConnectionFactory::getPassword,
                                    return_value_policy<return_by_value>()),
                      &ActiveMQConnectionFactory::setPassword)
        .add_property("brokerURL",
                      make_function(&ActiveMQConnectionFactory::getBrokerURL,
                                    return_value_policy<return_by_value>()),
                      &ActiveMQConnectionFactory::setBrokerURL)
        ;
}
