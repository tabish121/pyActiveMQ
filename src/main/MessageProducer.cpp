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

    py::class_<MessageProducer, py::bases<Closeable>, boost::noncopyable>(
        "MessageProducer", py::no_init)
        .def("send", MessageProducer_send1, py::arg("message"))
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
                      &MessageProducer::setDeliveryMode)
        .add_property("disableMessageID",
                      &MessageProducer::getDisableMessageID,
                      &MessageProducer::setDisableMessageID)
        .add_property("disableMessageTimeStamp",
                      &MessageProducer::getDisableMessageTimeStamp,
                      &MessageProducer::setDisableMessageTimeStamp)
        .add_property("priority",
                      &MessageProducer::getPriority,
                      &MessageProducer::setPriority)
        .add_property("timeToLive",
                      &MessageProducer::getTimeToLive,
                      &MessageProducer::setTimeToLive)
        ;
}
