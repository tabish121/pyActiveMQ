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

// TODO look at issues with the GIL here when multiple session threads
// call into the Python code (could happen with Openwire?)

#include <boost/python.hpp>
#include <cms/MessageListener.h>

using namespace boost::python;
using cms::MessageListener;
using cms::Message;

struct MessageListenerWrap : MessageListener, wrapper<MessageListener>
{
    virtual void onMessage(const Message* message)
    {
        // should be: this->getOverride("onMessage")(message);
        // but that doesn't work with Visual C++
        call<void>(this->get_override("onMessage").ptr(), message);
    }
};

void export_MessageListener()
{
    class_<MessageListenerWrap, boost::noncopyable>("MessageListener")
        .def("onMessage", pure_virtual(&MessageListener::onMessage))
        ;
}
