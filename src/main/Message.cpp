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
#include <cms/Message.h>

using namespace boost::python;
using cms::Message;

// TODO maybe some of these properties should be read only?

void export_Message()
{
    class_<Message, boost::noncopyable>("Message", no_init)
        // TODO clone?
        .def("acknowledge", &Message::acknowledge)
        .def("clearBody", &Message::clearBody)
        .def("clearProperties", &Message::clearProperties)
        .add_property("propertyNames", &Message::getPropertyNames)
        .def("propertyExists", &Message::propertyExists)
        .def("getBooleanProperty", &Message::getBooleanProperty)
        .def("getByteProperty", &Message::getByteProperty)
        .def("getDoubleProperty", &Message::getDoubleProperty)
        .def("getFloatProperty", &Message::getFloatProperty)
        .def("getIntProperty", &Message::getIntProperty)
        .def("getLongProperty", &Message::getLongProperty)
        .def("getShortProperty", &Message::getShortProperty)
        .def("getStringProperty", &Message::getStringProperty)
        .def("setBooleanProperty", &Message::setBooleanProperty)
        .def("setByteProperty", &Message::setByteProperty)
        .def("setDoubleProperty", &Message::setDoubleProperty)
        .def("setFloatProperty", &Message::setFloatProperty)
        .def("setIntProperty", &Message::setIntProperty)
        .def("setLongProperty", &Message::setLongProperty)
        .def("setShortProperty", &Message::setShortProperty)
        .def("setStringProperty", &Message::setStringProperty)
        .add_property("correlationId", &Message::getCMSCorrelationId, &Message::setCMSCorrelationId)
        .add_property("deliveryMode", &Message::getCMSDeliveryMode, &Message::setCMSDeliveryMode)
        .add_property("destination",
                      make_function(&Message::getCMSDestination, return_internal_reference<>()),
                      make_function(&Message::setCMSDestination, with_custodian_and_ward<1,2>()))
        .add_property("expiration", &Message::getCMSExpiration, &Message::setCMSExpiration)
        .add_property("messageId", &Message::getCMSMessageId, &Message::setCMSMessageId)
        .add_property("priority", &Message::getCMSPriority, &Message::setCMSPriority)
        .add_property("redelivered", &Message::getCMSRedelivered, &Message::setCMSRedelivered)
        .add_property("replyTo",
                      make_function(&Message::getCMSReplyTo, return_internal_reference<>()),
                      make_function(&Message::setCMSReplyTo, with_custodian_and_ward<1,2>()))
        .add_property("timeStamp", &Message::getCMSTimeStamp, &Message::setCMSTimeStamp)
        .add_property("messageType", &Message::getCMSMessageType, &Message::setCMSMessageType)
        ;
}
