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
#include <boost/python/manage_new_object.hpp>
#include <boost/python/dict.hpp>

#include <cms/ObjectMessage.h>

namespace py = boost::python;

using cms::ObjectMessage;
using cms::Message;

static ObjectMessage* ObjectMessage_deepcopy(ObjectMessage* This, py::dict memo)
{
    return dynamic_cast<ObjectMessage*>(This->clone());
}

void export_ObjectMessage()
{
    py::class_<ObjectMessage, py::bases<Message>, boost::noncopyable>("ObjectMessage", py::no_init)
        .def("__deepcopy__", ObjectMessage_deepcopy, py::return_value_policy<py::manage_new_object>());
}
