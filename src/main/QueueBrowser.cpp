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

#include <cms/QueueBrowser.h>

namespace py = boost::python;

using cms::Closeable;
using cms::QueueBrowser;

void export_QueueBrowser()
{
    py::class_<QueueBrowser, py::bases<Closeable>, boost::noncopyable>("QueueBrowser", py::no_init)
        // TODO check if this is an internal reference or a new object
        .add_property("queue", make_function(&QueueBrowser::getQueue, py::return_internal_reference<>()))
        .add_property("messageSelector", &QueueBrowser::getMessageSelector)
#if 0
        // TODO apply std_vector_to_tuple and to_python_Message to return value
        .add_property("enumeration", &QueueBrowser::getEnumeration)
#endif
        ;
}
