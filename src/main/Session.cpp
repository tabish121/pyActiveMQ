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
#include <boost/python/enum.hpp>
#include <boost/python/manage_new_object.hpp>

#include <cms/Session.h>

namespace py = boost::python;

using cms::Session;
using cms::Closeable;
using cms::MessageConsumer;
using cms::Destination;
using cms::TextMessage;
using cms::BytesMessage;

static const char* Session_docstring =
    "A C{Session} object is a single-threaded context for producing and consuming "
    "messages. \n\nA session serves several purposes:\n\n"
    " - It is a factory for its message producers and consumers.\n\n"
    " - It supplies provider-optimized message factories.\n\n"
    " - It is a factory for TemporaryTopics and TemporaryQueues.\n\n"
    " - It provides a way to create L{Queue} or L{Topic} objects for those clients "
    "that need to dynamically manipulate provider-specific destination names.\n\n"
    " - It supports a single series of transactions that combine work spanning its "
    "producers and consumers into atomic units.\n\n - It defines a serial order for "
    "the messages it consumes and the messages it produces.\n\n - It retains "
    "messages it consumes until they have been acknowledged.\n\n - It serializes "
    "execution of message listeners registered with its message consumers.";
static const char* Session_commit_docstring =
    "Commits all messages done in this transaction and releases any locks "
    "currently held.";
static const char* Session_rollback_docstring =
    "Rolls back all messages done in this transaction and releases any locks "
    "currently held.";
static const char* Session_unsubscribe_docstring =
    "Unsubscribes a durable subscription that has been created by a client.\n\n"
    "This method deletes the state being maintained on behalf of the subscriber "
    "by its provider. It is erroneous for a client to delete a durable "
    "subscription while there is an active L{MessageConsumer} or subscriber "
    "for the subscription, or while a consumed message is part of a pending "
    "transaction or has not been acknowledged in the session.";
static const char* Session_createConsumer1_docstring =
    "Creates a L{MessageConsumer} for the specified destination.";
static const char* Session_createConsumer2_docstring =
    "Creates a L{MessageConsumer} for the specified destination, using a "
    "message selector.";
static const char* Session_createConsumer3_docstring = 
    "Creates a L{MessageConsumer} for the specified destination, using a "
    "message selector.";
static const char* Session_createDurableConsumer_docstring =
    "Creates a durable subscriber to the specified topic, using a "
    "message selector.";
static const char* Session_createProducer_docstring =
    "Creates a L{MessageProducer} to send messages to the specified destination.";
static const char* Session_createTopic_docstring =
    "Creates a topic identity given a L{Topic} name.";
static const char* Session_createQueue_docstring =
    "Creates a queue identity given a L{Queue} name.";
static const char* Session_createTemporaryQueue_docstring =
    "Creates a L{TemporaryQueue} object.";
static const char* Session_createTemporaryTopic_docstring =
    "Creates a L{TemporaryTopic} object.";
static const char* Session_createMessage_docstring =
    "Creates a new L{Message}.";
static const char* Session_createTextMessage0_docstring =
    "Creates a new L{TextMessage}.";
static const char* Session_createTextMessage1_docstring =
    "Creates a new L{TextMessage} and set the text to the given value.";
static const char* Session_createBytesMessage_docstring =
    "Creates a new L{BytesMessage}.";
static const char* Session_createMapMessage_docstring =
    "Creates a new L{MapMessage}.";
static const char* Session_transacted_docstring = "Returns if the C{Session} is a transacted session.";
static const char* Session_acknowledgeMode_docstring = "Returns the acknowledgement mode of the C{Session}.";
static const char* AcknowledgeMode_docstring = "Returns the acknowledgement mode of the session.";

void export_Session()
{
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

    using py::return_value_policy;
    using py::manage_new_object;
    using py::with_custodian_and_ward_postcall;
    py::class_<Session, py::bases<Closeable>, boost::noncopyable>("Session", Session_docstring, py::no_init)
        .def("commit", &Session::commit, Session_commit_docstring)
        .def("rollback", &Session::rollback, Session_rollback_docstring)
        .def("unsubscribe", &Session::unsubscribe, py::arg("name"), Session_unsubscribe_docstring)
        .def("createConsumer",
             Session_createConsumer1,
             return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >(),
             py::arg("destination"),
             Session_createConsumer1_docstring)
        .def("createConsumer",
             Session_createConsumer2,
             return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >(),
             (py::arg("destination"), py::arg("selector")),
             Session_createConsumer2_docstring)
        .def("createConsumer",
             Session_createConsumer3,
             return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >(),
             (py::arg("destination"), py::arg("selector"), py::arg("nolocal")),
             Session_createConsumer3_docstring)
        .def("createDurableConsumer",
             &Session::createDurableConsumer,
             return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >(),
             (py::arg("destination"), py::arg("name"), py::arg("selector"), py::arg("nolocal")),
             Session_createDurableConsumer_docstring)
        .def("createProducer",
             &Session::createProducer,
             return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >(),
             py::arg("destination"),
             Session_createProducer_docstring)
        .def("createTopic",
             &Session::createTopic,
             return_value_policy<manage_new_object>(),
             py::arg("name"),
             Session_createTopic_docstring)
        .def("createQueue", &Session::createQueue,
             return_value_policy<manage_new_object>(),
             py::arg("name"),
             Session_createQueue_docstring)
        .def("createTemporaryTopic",
             &Session::createTemporaryTopic,
             return_value_policy<manage_new_object>(),
             py::arg("name"),
             Session_createTemporaryTopic_docstring)
        .def("createTemporaryQueue",
             &Session::createTemporaryQueue,
             return_value_policy<manage_new_object>(),
             py::arg("name"),
             Session_createTemporaryQueue_docstring)
        .def("createMessage",
             &Session::createMessage,
             return_value_policy<manage_new_object>(),
             Session_createMessage_docstring)
        .def("createTextMessage", Session_createTextMessage0, return_value_policy<manage_new_object>(), Session_createTextMessage0_docstring)
        .def("createTextMessage", Session_createTextMessage1, return_value_policy<manage_new_object>(), Session_createTextMessage1_docstring)
        .def("createBytesMessage", Session_createBytesMessage0, return_value_policy<manage_new_object>(), Session_createBytesMessage_docstring)
        .def("createMapMessage", &Session::createMapMessage, return_value_policy<manage_new_object>(), Session_createMapMessage_docstring)
        .add_property("acknowledgeMode", &Session::getAcknowledgeMode, Session_acknowledgeMode_docstring)
        .add_property("transacted", &Session::isTransacted, Session_transacted_docstring)
        ;

    py::enum_<Session::AcknowledgeMode>("AcknowledgeMode")
        .value("AUTO_ACKNOWLEDGE", Session::AUTO_ACKNOWLEDGE)
        .value("DUPS_OK_ACKNOWLEDGE", Session::DUPS_OK_ACKNOWLEDGE)
        .value("CLIENT_ACKNOWLEDGE", Session::CLIENT_ACKNOWLEDGE)
        .value("SESSION_TRANSACTED", Session::SESSION_TRANSACTED)
        ;
}
