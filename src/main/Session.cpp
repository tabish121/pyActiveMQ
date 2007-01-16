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
#include <cms/Session.h>

using namespace boost::python;
using cms::Session;
using cms::Closeable;
using cms::MessageConsumer;
using cms::Destination;
using cms::TextMessage;
using cms::BytesMessage;

MessageConsumer* (Session::*Session_createConsumer1)(const Destination*) =
    &Session::createConsumer;
MessageConsumer* (Session::*Session_createConsumer2)(
    const Destination*, const std::string&) =
    &Session::createConsumer;
MessageConsumer* (Session::*Session_createConsumer3)(
    const Destination*, const std::string&, bool) =
    &Session::createConsumer;
TextMessage* (Session::*Session_createTextMessage0)() =
    &Session::createTextMessage;
TextMessage* (Session::*Session_createTextMessage1)(const std::string&) =
    &Session::createTextMessage;
BytesMessage* (Session::*Session_createBytesMessage0)() =
    &Session::createBytesMessage;
// TODO 2-argument createBytesMessage

void export_Session()
{
    class_<Session, bases<Closeable>, boost::noncopyable>("Session", no_init)
        .def("commit", &Session::commit)
        .def("rollback", &Session::rollback)
        .def("createConsumer", Session_createConsumer1, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createConsumer", Session_createConsumer2, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createConsumer", Session_createConsumer3, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createDurableConsumer", &Session::createDurableConsumer, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createProducer", &Session::createProducer, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createTopic", &Session::createTopic, return_value_policy<manage_new_object>())
        .def("createQueue", &Session::createQueue, return_value_policy<manage_new_object>())
        .def("createTemporaryTopic", &Session::createTemporaryTopic, return_value_policy<manage_new_object>())
        .def("createTemporaryQueue", &Session::createTemporaryQueue, return_value_policy<manage_new_object>())
        .def("createMessage", &Session::createMessage, return_value_policy<manage_new_object>())
        .def("createTextMessage", Session_createTextMessage0, return_value_policy<manage_new_object>())
        .def("createTextMessage", Session_createTextMessage1, return_value_policy<manage_new_object>())
        .def("createBytesMessage", Session_createBytesMessage0, return_value_policy<manage_new_object>())
        .def("createMapMessage", &Session::createMapMessage, return_value_policy<manage_new_object>())
        .add_property("acknowledgeMode", &Session::getAcknowledgeMode)
        .add_property("transacted", &Session::isTransacted)
        ;

    enum_<Session::AcknowledgeMode>("AcknowledgeMode")
        .value("AUTO_ACKNOWLEDGE", Session::AUTO_ACKNOWLEDGE)
        .value("DUPS_OK_ACKNOWLEDGE", Session::DUPS_OK_ACKNOWLEDGE)
        .value("CLIENT_ACKNOWLEDGE", Session::CLIENT_ACKNOWLEDGE)
        .value("SESSION_TRANSACTED", Session::SESSION_TRANSACTED)
        ;
}
