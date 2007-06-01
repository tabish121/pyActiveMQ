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

#include <cms/Destination.h>
#include <cms/Topic.h>
#include <cms/Queue.h>
#include <cms/TemporaryTopic.h>
#include <cms/TemporaryQueue.h>

namespace py = boost::python;

using cms::Destination;
using cms::Topic;
using cms::Queue;
using cms::TemporaryTopic;
using cms::TemporaryQueue;

bool Destination__eq__(const Destination& self, const Destination& other)
{
    if (self.getDestinationType() != other.getDestinationType()) {
        return false;
    }
    switch (self.getDestinationType())
    {
    case Destination::TOPIC:
    {
        const Topic& selfTopic = dynamic_cast<const Topic&>(self);
        const Topic& otherTopic = dynamic_cast<const Topic&>(other);
        return selfTopic.getTopicName() == otherTopic.getTopicName();
    }
    case Destination::QUEUE:
    {
        const Queue& selfQueue = dynamic_cast<const Queue&>(self);
        const Queue& otherQueue = dynamic_cast<const Queue&>(other);
        return selfQueue.getQueueName() == otherQueue.getQueueName();
    }
    case Destination::TEMPORARY_TOPIC:
    {
        const TemporaryTopic& selfTopic =
            dynamic_cast<const TemporaryTopic&>(self);
        const TemporaryTopic& otherTopic =
            dynamic_cast<const TemporaryTopic&>(other);
        return selfTopic.getTopicName() == otherTopic.getTopicName();
    }
    case Destination::TEMPORARY_QUEUE:
    {
        const TemporaryQueue& selfQueue =
            dynamic_cast<const TemporaryQueue&>(self);
        const TemporaryQueue& otherQueue =
            dynamic_cast<const TemporaryQueue&>(other);
        return selfQueue.getQueueName() == otherQueue.getQueueName();
    }
    default:
        Py_FatalError("Invalid Destination type encountered in Destinations");
        return false;
    }
}

void export_Destinations()
{
    py::class_<Destination, boost::noncopyable>("Destination", py::no_init)
        .add_property("destinationType", &Destination::getDestinationType)
        .def("__str__", &Destination::toProviderString)
        .def("__repr__", &Destination::toProviderString)
        .def("__eq__", Destination__eq__)
        .add_property("providerString", &Destination::toProviderString)
        ;

    py::enum_<Destination::DestinationType>("DestinationType")
        .value("TOPIC", Destination::TOPIC)
        .value("QUEUE", Destination::QUEUE)
        .value("TEMPORARY_TOPIC", Destination::TEMPORARY_TOPIC)
        .value("TEMPORARY_QUEUE", Destination::TEMPORARY_QUEUE)
        ;

    py::class_<Topic, py::bases<Destination>, boost::noncopyable>("Topic", py::no_init)
        .add_property("name", &Topic::getTopicName)
        ;

    py::class_<Queue, py::bases<Destination>, boost::noncopyable>("Queue", py::no_init)
        .add_property("name", &Queue::getQueueName)
        ;

    py::class_<TemporaryTopic, py::bases<Destination>, boost::noncopyable>("TemporaryTopic", py::no_init)
        .add_property("name", &TemporaryTopic::getTopicName)
        ;

    py::class_<TemporaryQueue, py::bases<Destination>, boost::noncopyable>("TemporaryQueue", py::no_init)
        .add_property("name", &TemporaryQueue::getQueueName)
        ;
}
