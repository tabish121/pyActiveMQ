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

#include "pyactivemq.h"

#include <boost/python/class.hpp>
#include <boost/python/object/pointer_holder.hpp>

#include <cms/Message.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/TemporaryTopic.h>
#include <cms/TemporaryQueue.h>

namespace py = boost::python;

using cms::Message;
using cms::Destination;
using cms::Topic;
using cms::Queue;
using cms::TemporaryTopic;
using cms::TemporaryQueue;

static const char* Message_docstring =
    "Root of all messages.\n\nAs in JMS, a message is comprised of 3 parts: "
    "CMS-specific headers, user-defined properties, and the body.";
static const char* Message_acknowledge_docstring =
    "Acknowledges all consumed messages of the session of this consumed message.\n\n"
    "All consumed CMS messages support the acknowledge method for use when a client "
    "has specified that its CMS session's consumed messages are to be explicitly "
    "acknowledged. By invoking acknowledge on a consumed message, a client "
    "acknowledges all messages consumed by the session that the message was delivered"
    " to.\n\nCalls to acknowledge are ignored for both transacted sessions and "
    "sessions specified to use implicit acknowledgement modes.\n\nA client may "
    "individually acknowledge each message as it is consumed, or it may choose to "
    "acknowledge messages as an application-defined group (which is done by calling "
    "acknowledge on the last received message of the group, thereby acknowledging "
    "all messages consumed by the session.)\n\nMessages that have been received but "
    "not acknowledged may be redelivered.";
static const char* Message_clearBody_docstring =
    "Clears out the body of the message.\n\nThis does not clear the headers or properties.";
static const char* Message_clearProperties_docstring =
    "Clears out the message body.\n\nClearing a message's body does not clear its "
    "header values or property entries.\n\nIf this message body was read-only, "
    "calling this method leaves the message body in the same state as an empty body "
    "in a newly created message.";
static const char* Message_propertyNames_docstring = "Retrieves the complete set of property names currently in this message.";
static const char* Message_propertyExists_docstring = "Indicates whether or not a given property exists.";
static const char* Message_getBooleanProperty_docstring = "Gets a boolean property.";
static const char* Message_getByteProperty_docstring = "Gets a byte property.";
static const char* Message_getDoubleProperty_docstring = "Gets a double property.";
static const char* Message_getFloatProperty_docstring = "Gets a float property.";
static const char* Message_getIntProperty_docstring = "Gets a int property.";
static const char* Message_getLongProperty_docstring = "Gets a long property.";
static const char* Message_getShortProperty_docstring = "Gets a short property.";
static const char* Message_getStringProperty_docstring = "Gets a string property.";
static const char* Message_setBooleanProperty_docstring = "Sets a boolean property.";
static const char* Message_setByteProperty_docstring = "Sets a byte property.";
static const char* Message_setDoubleProperty_docstring = "Sets a double property.";
static const char* Message_setFloatProperty_docstring = "Sets a float property.";
static const char* Message_setIntProperty_docstring = "Sets a int property.";
static const char* Message_setLongProperty_docstring = "Sets a long property.";
static const char* Message_setShortProperty_docstring = "Sets a short property.";
static const char* Message_setStringProperty_docstring = "Sets a string property.";
static const char* Message_deliveryMode_docstring = "Gets the L{DeliveryMode} for this message.";
static const char* Message_expiration_docstring =
    "Gets the message's expiration value.\n\nWhen a message is sent, the expiration "
    "header field is left unassigned. After completion of the send or publish method, "
    "it holds the expiration time of the message. This is the sum of the time-to-live "
    "value specified by the client and the GMT at the time of the send or publish.\n\n"
    "If the time-to-live is specified as zero, expiration is set to zero to indicate "
    "that the message does not expire.\n\nWhen a message's expiration time is reached,"
    " a provider should discard it.";
static const char* Message_messageID_docstring =
    "The C{messageID} header field contains a value that uniquely identifies each message"
    " sent by a provider.\n\nWhen a message is sent, messageID can be ignored. When the "
    "send or publish method returns, it contains a provider-assigned value.\n\nA "
    "messageID is a C{String} value that should function as a unique key for "
    "identifying messages in a historical repository. The exact scope of uniqueness is "
    "provider-defined. It should at least cover all messages for a specific "
    "installation of a provider, where an installation is some connected set of "
    "message routers.\n\nAll C{messageID} values must start with the prefix 'C{ID:}'. "
    "Uniqueness of message ID values across different providers is not required.\n\n"
    "Since message IDs take some effort to create and increase a message's size, some "
    "CMS providers may be able to optimize message overhead if they are given a hint "
    "that the message ID is not used by an application. By setting the "
    "L{MessageProducer.disableMessageID} method, a CMS client enables this potential "
    "optimization for all messages sent by that message producer. If the CMS provider "
    "accepts this hint, these messages must have the message ID set to null; if the "
    "provider ignores the hint, the message ID must be set to its normal unique value.";
static const char* Message_priority_docstring =
    "Gets the message priority level.\n\nThe CMS API defines ten levels of priority "
    "value, with 0 as the lowest priority and 9 as the highest. In addition, clients "
    "should consider priorities 0-4 as gradations of normal priority and priorities 5-9 "
    "as gradations of expedited priority.\n\nThe CMS API does not require that a "
    "provider strictly implements priority ordering of messages; however, it should do "
    "its best to deliver expedited messages ahead of normal messages.";
static const char* Message_redelivered_docstring =
    "Gets an indication of whether this message is being redelivered.\n\nIf a client "
    "receives a message with the redelivered field set, it is likely, but not "
    "guaranteed, that this message was delivered earlier but that its receipt was not "
    "acknowledged at that time.";
static const char* Message_timestamp_docstring =
    "Gets the message timestamp.\n\nThe C{timestamp} header field contains the time a "
    "message was handed off to a provider to be sent. It is not the time the message was"
    " actually transmitted, because the actual send may occur later due to transactions "
    "or other client-side queueing of messages.\n\nWhen a message is sent, C{timestamp} "
    "is ignored. When the send or publish method returns, it contains a time value "
    "somewhere in the interval between the call and the return. The value is in the "
    "format of a normal millis time value in the Java programming language.\n\nSince "
    "timestamps take some effort to create and increase a message's size, some CMS "
    "providers may be able to optimize message overhead if they are given a hint that "
    "the timestamp is not used by an application. By setting "
    "L{MessageProducer.disableMessageTimeStamp}, a CMS client enables this potential "
    "optimization for all messages sent by that message producer. If the CMS provider "
    "accepts this hint, these messages must have the timestamp set to zero; if the "
    "provider ignores the hint, the timestamp must be set to its normal value.";
static const char* Message_replyTo_docstring =
    "The L{Destination} object to which a reply to this message should be sent."
    "\n\nThe C{replyTo} header field contains the destination where a reply to "
    "the current message should be sent. If it is null, no reply is expected. "
    "The destination may be either a L{Queue} object or a L{Topic} object.\n\n"
    "Messages sent with a null C{replyTo} value may be a notification of some "
    "event, or they may just be some data the sender thinks is of interest.\n\n"
    "Messages with a C{replyTo} value typically expect a response. A response "
    "is optional; it is up to the client to decide. These messages are called "
    "requests. A message sent in response to a request is called a reply.\n\n"
    "In some cases a client may wish to match a request it sent earlier with "
    "a reply it has just received. The client can use the L{correlationID} "
    "header field for this purpose.";
static const char* Message_destination_docstring =
    "Returns the L{Destination} object for this message.\n\nThe destination header "
    "field contains the destination to which the message is being sent.\n\nWhen a "
    "message is sent, this field is ignored. After completion of the send or "
    "publish method, the field holds the destination specified by the method.\n\n"
    "When a message is received, its C{destination} value must be equivalent to "
    "the value assigned when it was sent.";
static const char* Message_correlationID_docstring =
    "The correlation ID for the message.\n\nA client can use the C{correlationID} "
    "header field to link one message with another. A typical use is to link a "
    "response message with its request message.\n\nC{correlationID} can hold one "
    "of the following:\n\n"
    " - A provider-specific message ID\n\n"
    " - An application-specific string\n\n"
    " - A provider-native byte value\n\n"
    "Since each message sent by a CMS provider is assigned a message ID value, "
    "it is convenient to link messages via message ID. All message ID values "
    "must start with the 'C{ID:}' prefix.\n\nIn some cases, an application "
    "(made up of several clients) needs to use an application-specific value "
    "for linking messages. For instance, an application may use "
    "C{correlationID} to hold a value referencing some external information. "
    "Application-specified values must not start with the 'C{ID:}' prefix; this "
    "is reserved for provider-generated message ID values.\n\nIf a provider "
    "supports the native concept of correlation ID, a CMS client may need to "
    "assign specific C{correlationID} values to match those expected by clients "
    "that do not use the CMS API. A byte value is used for this purpose. CMS "
    "providers without native correlation ID values are not required to "
    "support byte values. The use of a byte value for correlationID is "
    "non-portable.";
static const char* Message_type_docstring =
    "Sets the message type.\n\nSome CMS providers use a message repository that "
    "contains the definitions of messages sent by applications. The C{type} header "
    "field may reference a message's definition in the provider's repository.\n\n"
    "The CMS API does not define a standard message definition repository, nor "
    "does it define a naming policy for the definitions it contains.\n\nSome "
    "messaging systems require that a message type definition for each application "
    "message be created and that each message specify its type. In order to work "
    "with such CMS providers, CMS clients should assign a value to C{type}, "
    "whether the application makes use of it or not. This ensures that the field "
    "is properly set for those providers that require it.\n\nTo ensure portability, "
    "CMS clients should use symbolic values for C{type} that can be configured at "
    "installation time to the values defined in the current provider's message "
    "repository. If string literals are used, they may not be valid type names "
    "for some CMS providers.";

void export_Message()
{
    py::class_<Message, boost::noncopyable>("Message", Message_docstring, py::no_init)
        .def("acknowledge", &Message::acknowledge, Message_acknowledge_docstring)
        .def("clearBody", &Message::clearBody, Message_clearBody_docstring)
        .def("clearProperties", &Message::clearProperties, Message_clearProperties_docstring)
        .add_property("propertyNames", &Message::getPropertyNames, Message_propertyNames_docstring)
        .def("propertyExists", &Message::propertyExists, Message_propertyExists_docstring)
        .def("getBooleanProperty", &Message::getBooleanProperty, Message_getBooleanProperty_docstring)
        .def("getByteProperty", &Message::getByteProperty, Message_getByteProperty_docstring)
        .def("getDoubleProperty", &Message::getDoubleProperty, Message_getDoubleProperty_docstring)
        .def("getFloatProperty", &Message::getFloatProperty, Message_getFloatProperty_docstring)
        .def("getIntProperty", &Message::getIntProperty, Message_getIntProperty_docstring)
        .def("getLongProperty", &Message::getLongProperty, Message_getLongProperty_docstring)
        .def("getShortProperty", &Message::getShortProperty, Message_getShortProperty_docstring)
        .def("getStringProperty", &Message::getStringProperty, Message_getStringProperty_docstring)
        .def("setBooleanProperty", &Message::setBooleanProperty, Message_setBooleanProperty_docstring)
        .def("setByteProperty", &Message::setByteProperty, Message_setByteProperty_docstring)
        .def("setDoubleProperty", &Message::setDoubleProperty, Message_setDoubleProperty_docstring)
        .def("setFloatProperty", &Message::setFloatProperty, Message_setFloatProperty_docstring)
        .def("setIntProperty", &Message::setIntProperty, Message_setIntProperty_docstring)
        .def("setLongProperty", &Message::setLongProperty, Message_setLongProperty_docstring)
        .def("setShortProperty", &Message::setShortProperty, Message_setShortProperty_docstring)
        .def("setStringProperty", &Message::setStringProperty, Message_setStringProperty_docstring)
        // read-only properties
        .add_property("deliveryMode", &Message::getCMSDeliveryMode, Message_deliveryMode_docstring)
        .add_property("expiration", &Message::getCMSExpiration, Message_expiration_docstring)
        .add_property("messageID", &Message::getCMSMessageID, Message_messageID_docstring)
        .add_property("priority", &Message::getCMSPriority, Message_priority_docstring)
        .add_property("redelivered", &Message::getCMSRedelivered, Message_redelivered_docstring)
        .add_property("timestamp", &Message::getCMSTimestamp, Message_timestamp_docstring)
        // read-write properties
        .add_property("replyTo",
                      make_function(&Message::getCMSReplyTo, py::return_internal_reference<>()),
                      make_function(&Message::setCMSReplyTo, py::with_custodian_and_ward<1, 2>()),
                      Message_replyTo_docstring)
        .add_property("destination",
                      make_function(&Message::getCMSDestination, py::return_internal_reference<>()),
                      make_function(&Message::setCMSDestination, py::with_custodian_and_ward<1, 2>()),
                      Message_destination_docstring)
        .add_property("correlationID", &Message::getCMSCorrelationID, &Message::setCMSCorrelationID,
                      Message_correlationID_docstring)
        .add_property("type", &Message::getCMSType, &Message::setCMSType, Message_type_docstring)
        ;
}
