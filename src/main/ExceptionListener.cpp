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
#include <cms/ExceptionListener.h>

using namespace boost::python;
using cms::ExceptionListener;
using cms::CMSException;

struct ExceptionListenerWrap : ExceptionListener, wrapper<ExceptionListener>
{
    virtual void onException(const CMSException& ex)
    {
        // should be: this->getOverride("onException")(ex);
        // but that doesn't work with Visual C++
        call<void>(this->get_override("onException").ptr(), ex);
    }
};

void export_ExceptionListener()
{
    class_<ExceptionListenerWrap, boost::noncopyable>("ExceptionListener")
        .def("onException", pure_virtual(&ExceptionListener::onException))
        ;
}
