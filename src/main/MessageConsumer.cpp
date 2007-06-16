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

#include <cms/MessageConsumer.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>

namespace py = boost::python;

using cms::MessageConsumer;
using cms::Closeable;
using cms::Message;

template<class T>
struct to_python_Message
{
    template <class U>
    inline PyObject* operator()(U const* ref) const
    {
        if (ref == 0) {
            return py::incref(boost::python::detail::none());
        }
        U* const p = const_cast<U*>(ref);
        if (dynamic_cast<cms::BytesMessage*>(p) != 0) {
            return make_owning_holder::execute(dynamic_cast<cms::BytesMessage*>(p));
        }
        else if (dynamic_cast<cms::TextMessage*>(p) != 0) {
            return make_owning_holder::execute(dynamic_cast<cms::TextMessage*>(p));
        }
        else if (dynamic_cast<cms::MapMessage*>(p) != 0) {
            return make_owning_holder::execute(dynamic_cast<cms::MapMessage*>(p));
        }
        else if (dynamic_cast<cms::Message*>(p) != 0) {
            return make_owning_holder::execute(dynamic_cast<cms::Message*>(p));
        }
        Py_FatalError("Invalid Message type encountered in to_python_Message");
        return 0;
    }
};

struct manage_new_Message
{
    template<class T>
    struct apply
    {
        typedef to_python_Message<T> type;
    };
};

void export_MessageConsumer()
{
    Message* (MessageConsumer::*MessageConsumer_receive0)() = &MessageConsumer::receive;
    Message* (MessageConsumer::*MessageConsumer_receive1)(int) = &MessageConsumer::receive;
    py::class_<MessageConsumer, py::bases<Closeable>, boost::noncopyable>("MessageConsumer", py::no_init)
        .def("receive", MessageConsumer_receive0, py::return_value_policy<manage_new_Message>())
        .def("receive",
             MessageConsumer_receive1,
             py::return_value_policy<manage_new_Message>(),
             py::arg("timeout"))
        .def("receiveNoWait",
             &MessageConsumer::receiveNoWait,
             py::return_value_policy<manage_new_Message>())
        .add_property("messageListener",
                      make_function(&MessageConsumer::getMessageListener, py::return_internal_reference<>()),
                      make_function(&MessageConsumer::setMessageListener, py::with_custodian_and_ward<1,2>()))
        .add_property("messageSelector", &MessageConsumer::getMessageSelector)
        ;
}
