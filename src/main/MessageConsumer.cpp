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

#include <cms/MessageConsumer.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>

using namespace boost::python;
using namespace boost::python::detail;
using cms::MessageConsumer;
using cms::Closeable;
using cms::Message;

template <class T>
struct to_python_Message
{
    template <class U>
    inline PyObject* operator()(U const* ref) const
    {
        if (ref == 0)
        {
            return boost::python::detail::none();
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
        Py_FatalError("invalid Message type encountered in MessageConsumer");
        return 0;
    }
};

struct manage_new_Message
{
    template <class T>
    struct apply
    {
        typedef to_python_Message<T> type;
    };
};

void export_MessageConsumer()
{
    Message* (MessageConsumer::*MessageConsumer_receive0)() = &MessageConsumer::receive;
    Message* (MessageConsumer::*MessageConsumer_receive1)(int) = &MessageConsumer::receive;
    class_<MessageConsumer, bases<Closeable>, boost::noncopyable>("MessageConsumer", no_init)
        .def("receive", MessageConsumer_receive0, return_value_policy<manage_new_Message>())
        .def("receive", MessageConsumer_receive1, return_value_policy<manage_new_Message>())
        .def("receiveNoWait",
             &MessageConsumer::receiveNoWait,
             return_value_policy<manage_new_Message>())
        .add_property("messageListener",
                      make_function(&MessageConsumer::getMessageListener, return_internal_reference<>()),
                      make_function(&MessageConsumer::setMessageListener, with_custodian_and_ward<1,2>()))
        .add_property("messageSelector", &MessageConsumer::getMessageSelector)
        ;
}
