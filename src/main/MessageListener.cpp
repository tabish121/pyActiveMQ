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
#include <boost/shared_ptr.hpp>

#include <cms/MessageListener.h>
#include <cms/Message.h>
#include <cms/BytesMessage.h>
#include <cms/TextMessage.h>
#include <cms/MapMessage.h>

using namespace boost::python;

using cms::MessageListener;
using cms::Message;
using cms::BytesMessage;
using cms::TextMessage;
using cms::MapMessage;

struct make_owning_holder
{
    template<class T>
    static PyObject* execute(T* p)
    {
        typedef boost::shared_ptr<T> smart_pointer;
        typedef objects::pointer_holder<smart_pointer, T> holder_t;
        smart_pointer ptr(const_cast<T*>(p));
        return objects::make_ptr_instance<T, holder_t>::execute(ptr);
    }
};

struct MessageListenerWrap : MessageListener, wrapper<MessageListener>
{
    virtual void onMessage(const Message* message)
    {
        PyGILState_STATE gstate = PyGILState_Ensure();

        if (dynamic_cast<const BytesMessage*>(message) != 0) {
            call_onMessage<BytesMessage>(message);
        }
        else if (dynamic_cast<const TextMessage*>(message) != 0) {
            call_onMessage<TextMessage>(message);
        }
        else if (dynamic_cast<const MapMessage*>(message) != 0) {
            call_onMessage<MapMessage>(message);
        }
        else {
            Py_FatalError("invalid Message type encountered in MessageListener");
        }

        PyGILState_Release(gstate);
    }

    template<class T>
    void call_onMessage(const Message* message)
    {
        // TODO consolidate with very similar code in MessageConsumer
        T* m = dynamic_cast<T*>(message->clone());
        PyObject* obj = to_python_indirect<T*, make_owning_holder>()(m);
        call<void>(this->get_override("onMessage").ptr(), handle<>(obj));
    }
};

void export_MessageListener()
{
    class_<MessageListenerWrap, boost::noncopyable>("MessageListener")
        .def("onMessage", pure_virtual(&MessageListener::onMessage))
        ;
}
