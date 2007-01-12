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

#if 1
cms::Connection* (ActiveMQConnectionFactory::*createConnection0)() =
    &ActiveMQConnectionFactory::createConnection;
#if 0
cms::Connection* (ActiveMQConnectionFactory::*createConnection1)(
    const std::string&) =
    &ActiveMQConnectionFactory::createConnection;
cms::Connection* (ActiveMQConnectionFactory::*createConnection2)(
    const std::string&, const std::string&) =
    &ActiveMQConnectionFactory::createConnection;
#endif
cms::Connection* (ActiveMQConnectionFactory::*createConnection3)(
    const std::string&, const std::string&, const std::string&) =
&ActiveMQConnectionFactory::createConnection;
#else
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ActiveMQConnectionFactory_overloads, createConnection, 0, 3)
#endif

cms::Session* (cms::Connection::*createSession0)() = &cms::Connection::createSession;

BOOST_PYTHON_MODULE(pyactivemq)
{
    class_<ActiveMQConnectionFactory>("ActiveMQConnectionFactory")
#if 0
        .def(init<const std::string&>())
        .def(init<const std::string&, const std::string&>())
        .def(init<const std::string&, const std::string&, const std::string&>())
        .def(init<const std::string&, const std::string&, const std::string&, const std::string&>())
#else
        .def(init<const std::string&, optional<const std::string&, const std::string&, const std::string&> >())
#endif

#if 0
        .def("createConnection", &ActiveMQConnectionFactory::createConnection,
             ActiveMQConnectionFactory_overloads(),
             return_value_policy<manage_new_object>())
#else
        .def("createConnection", createConnection0, return_value_policy<manage_new_object>())
#if 0
        .def("createConnection", createConnection1)
        .def("createConnection", createConnection2)
#endif
        .def("createConnection", createConnection3, return_value_policy<manage_new_object>())
#endif

        .def("setUsername", &ActiveMQConnectionFactory::setUsername)
        .def("getUsername", &ActiveMQConnectionFactory::getUsername,
             return_value_policy<return_by_value>())
#if 0
        .def("setPassword", &ActiveMQConnectionFactory::setPassword)
        .def("getPassword", &ActiveMQConnectionFactory::getPassword, return_internal_reference<>())
        .def("setBrokerURL", &ActiveMQConnectionFactory::setBrokerURL)
        .def("getBrokerURL", &ActiveMQConnectionFactory::getBrokerURL, return_internal_reference<>())
        .def("setClientId", &ActiveMQConnectionFactory::setClientId)
        .def("getClientId", &ActiveMQConnectionFactory::getClientId, return_internal_reference<>())
#endif
#if 0
        .add_property("brokerURL",
                      make_getter(&ActiveMQConnectionFactory::getBrokerURL,
                                  return_value_policy<return_by_value>()),
                      &ActiveMQConnectionFactory::setBrokerURL,
            "Broker URL")

#endif
        ;

    class_<cms::Connection, boost::noncopyable>("Connection", no_init)
        .add_property("clientId", &cms::Connection::getClientId)
        .def("createSession", createSession0, return_value_policy<manage_new_object>())
        ;

    class_<cms::Session, boost::noncopyable>("Session", no_init)
        .def("commit", &cms::Session::commit)
        .def("rollback", &cms::Session::rollback)
        .def("createTopic", &cms::Session::createTopic, return_value_policy<manage_new_object>())
        .def("createQueue", &cms::Session::createQueue, return_value_policy<manage_new_object>())
        .def("createTemporaryTopic", &cms::Session::createTemporaryTopic, return_value_policy<manage_new_object>())
        .def("createTemporaryQueue", &cms::Session::createTemporaryQueue, return_value_policy<manage_new_object>())
        .def("createMessage", &cms::Session::createMessage, return_value_policy<manage_new_object>())
#if 0
        .def("createBytesMessage", &cms::Session::createBytesMessage, return_value_policy<manage_new_object>())
        .def("createTextMessage", &cms::Session::createTextMessage, return_value_policy<manage_new_object>())
#endif
        .def("createMapMessage", &cms::Session::createMapMessage, return_value_policy<manage_new_object>())
#if 0
        .add_property("acknowledgeMode", &cms::Session::getAcknowlegdeMode)
#endif
        .add_property("transacted", &cms::Session::isTransacted)
        ;
}
