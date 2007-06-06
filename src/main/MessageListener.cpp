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
#include <boost/python/pure_virtual.hpp>

#include <cms/MessageListener.h>
#include <cms/Message.h>
#include <cms/BytesMessage.h>
#include <cms/TextMessage.h>
#include <cms/MapMessage.h>

#include "pyactivemq.h"

namespace py = boost::python;

using cms::MessageListener;
using cms::Message;
using cms::BytesMessage;
using cms::TextMessage;
using cms::MapMessage;

struct MessageListenerWrap : MessageListener, py::wrapper<MessageListener>
{
    virtual void onMessage(const Message* message)
    {
        PyGILState_STATE gstate = PyGILState_Ensure();

        if (dynamic_cast<const BytesMessage*>(message) != 0) {
            call_onMessage<BytesMessage>(message);
        } else if (dynamic_cast<const TextMessage*>(message) != 0) {
            call_onMessage<TextMessage>(message);
        } else if (dynamic_cast<const MapMessage*>(message) != 0) {
            call_onMessage<MapMessage>(message);
        } else {
            Py_FatalError("Invalid Message type encountered in MessageListener");
        }

        PyGILState_Release(gstate);
    }

    template<class T>
    void call_onMessage(const Message* message)
    {
        T* m = dynamic_cast<T*>(message->clone());
        PyObject* obj = py::to_python_indirect<T*, make_owning_holder>()(m);
        try {
            py::call<void>(this->get_override("onMessage").ptr(), py::handle<>(obj));
        } catch (const py::error_already_set) {
            // Catch and ignore exception that is thrown if Python
            // onMessage raised an exception. Print it to sys.stderr,
            // since there is nothing we can do about it here.
            PyErr_Print();
        }
    }
};

void export_MessageListener()
{
    py::class_<MessageListenerWrap, boost::noncopyable>("MessageListener")
        .def("onMessage", py::pure_virtual(&MessageListener::onMessage))
        ;
}
