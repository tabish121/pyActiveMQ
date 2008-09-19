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

#include <cms/MessageProducer.h>

namespace py = boost::python;

using cms::Closeable;
using cms::MessageProducer;
using cms::Message;
using cms::Destination;

static const char* MessageProducer_docstring =
    "A client uses a C{MessageProducer} object to send messages to a destination.\n\n"
    "A C{MessageProducer} object is created by passing a L{Destination} object to a "
    "message-producer creation method supplied by a session.\n\nA client also has the "
    "option of creating a message producer without supplying a destination. In this "
    "case, a destination must be provided with every send operation. A typical use "
    "for this kind of message producer is to send replies to requests using the "
    "request's replyTo destination.\n\nA client can specify a default delivery mode, "
    "priority, and time to live for messages sent by a message producer. It can also "
    "specify the delivery mode, priority, and time to live for an individual message."
    "\n\nA client can specify a time-to-live value in milliseconds for each message "
    "it sends. This value defines a message expiration time that is the sum of the "
    "message's time-to-live and the GMT when it is sent (for transacted sends, this "
    "is the time the client sends the message, not the time the transaction is "
    "committed).";
static const char* MessageProducer_send1_docstring =
    "Sends the message to the default producer destination, but does not take "
    "ownership of the message, caller must still destroy it.\n\nUses default "
    "values for deliveryMode, priority, and time to live.";
static const char* MessageProducer_deliveryMode_docstring =
    "The delivery mode for this producer.";
static const char* MessageProducer_disableMessageID_docstring =
    "Returns if L{Message} ids are disabled for this producer.";
static const char* MessageProducer_disableMessageTimeStamp_docstring =
    "Returns if L{Message} timestamps are disabled for this producer.";
static const char* MessageProducer_priority_docstring =
    "Returns the priority that this producer sends messages at.";
static const char* MessageProducer_timeToLive_docstring =
    "Returns the time to live that this producer sends messages "
    "with.\n\nThis value will be used if the time to live is not "
    "specified via the send method.";

void export_MessageProducer()
{
    void (MessageProducer::*MessageProducer_send1)(Message*) =
        &MessageProducer::send;
    void (MessageProducer::*MessageProducer_send4)(Message*, int, int, long long) =
        &MessageProducer::send;
    void (MessageProducer::*MessageProducer_send2)(const Destination*, Message*) =
        &MessageProducer::send;
    void (MessageProducer::*MessageProducer_send5)(
        const Destination*, Message*, int, int, long long) =
        &MessageProducer::send;

    py::class_<MessageProducer, py::bases<Closeable>, boost::noncopyable>("MessageProducer", MessageProducer_docstring, py::no_init)
        .def("send", MessageProducer_send1, py::arg("message"), MessageProducer_send1_docstring)
        .def("send", MessageProducer_send2,
             (py::arg("destination"), py::arg("message")))
        .def("send", MessageProducer_send4,
             (py::arg("message"), py::arg("deliveryMode"),
              py::arg("priority"), py::arg("timeToLive")))
        .def("send", MessageProducer_send5,
             (py::arg("destination"), py::arg("message"),
              py::arg("deliveryMode"), py::arg("priority"),
              py::arg("timeToLive")))
        .add_property("deliveryMode",
                      &MessageProducer::getDeliveryMode,
                      &MessageProducer::setDeliveryMode,
                      MessageProducer_deliveryMode_docstring)
        .add_property("disableMessageID",
                      &MessageProducer::getDisableMessageID,
                      &MessageProducer::setDisableMessageID,
                      MessageProducer_disableMessageID_docstring)
        .add_property("disableMessageTimeStamp",
                      &MessageProducer::getDisableMessageTimeStamp,
                      &MessageProducer::setDisableMessageTimeStamp,
                      MessageProducer_disableMessageTimeStamp_docstring)
        .add_property("priority",
                      &MessageProducer::getPriority,
                      &MessageProducer::setPriority,
                      MessageProducer_priority_docstring)
        .add_property("timeToLive",
                      &MessageProducer::getTimeToLive,
                      &MessageProducer::setTimeToLive,
                      MessageProducer_timeToLive_docstring)
        ;
}
