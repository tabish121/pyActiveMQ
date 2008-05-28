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

static const char* MessageConsumer_docstring = "A client uses a C{MessageConsumer} to receive messages from a destination.\n\n"
											   "A client may either synchronously receive a message consumer's messages or have "
											   "the consumer asynchronously deliver them as they arrive.\n\nFor synchronous receipt, "
											   "a client can request the next message from a message consumer using one of its receive "
											   "methods. There are several variations of receive that allow a client to poll or wait "
											   "for the next message.\n\nFor asynchronous delivery, a client can register a "
											   "L{MessageListener} object with a message consumer. As messages arrive at the message "
											   "consumer, it delivers them by calling the L{MessageListener}'s C{onMessage} method.";
static const char* MessageConsumer_receive0_docstring = "Synchronously receive a L{Message}.";
static const char* MessageConsumer_receive1_docstring = "Synchronously receive a L{Message}, time out after defined interval.\n\n"
														"Returns null if nothing read.";
static const char* MessageConsumer_receiveNoWait_docstring = "Receive a L{Message}, does not wait if there isn't a new message to "
															 "read, returns C{NULL} if nothing read.";
static const char* MessageConsumer_messageListener_docstring = "The L{MessageListener} that this class will send notifications on.";
static const char* MessageConsumer_messageSelector_docstring = "This message consumer's message selector expression.";

void export_MessageConsumer()
{
    Message* (MessageConsumer::*MessageConsumer_receive0)() = &MessageConsumer::receive;
    Message* (MessageConsumer::*MessageConsumer_receive1)(int) = &MessageConsumer::receive;

    py::class_<MessageConsumer, py::bases<Closeable>, boost::noncopyable>("MessageConsumer", MessageConsumer_docstring, py::no_init)
        .def("receive", MessageConsumer_receive0, py::return_value_policy<manage_new_Message>(), MessageConsumer_receive0_docstring)
        .def("receive",
             MessageConsumer_receive1,
             py::return_value_policy<manage_new_Message>(),
             py::arg("timeout"),
			 MessageConsumer_receive1_docstring)
        .def("receiveNoWait", 
             &MessageConsumer::receiveNoWait,
             py::return_value_policy<manage_new_Message>(),
			 MessageConsumer_receiveNoWait_docstring)
        .add_property("messageListener",
                      make_function(&MessageConsumer::getMessageListener, py::return_internal_reference<>()),
                      make_function(&MessageConsumer::setMessageListener, py::with_custodian_and_ward<1,2>()),
                      MessageConsumer_messageListener_docstring)
        .add_property("messageSelector",
                      &MessageConsumer::getMessageSelector,
                      MessageConsumer_messageSelector_docstring)
        ;
}
