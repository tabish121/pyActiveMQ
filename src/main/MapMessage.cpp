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
#include <cms/MapMessage.h>

using namespace boost::python;
using cms::MapMessage;
using cms::Message;

// TODO see if we can do something to make MapMessage work like a Python map

const char* MapMessage_getBytes(MapMessage& This, const std::string& name)
{
#if 0
    return reinterpret_cast<const char*>(This.getBytes(name));
#else
    return 0;
#endif
}

void export_MapMessage()
{
    class_<MapMessage, bases<Message>, boost::noncopyable>("MapMessage", no_init)
        .add_property("mapNames", &MapMessage::getMapNames)
        .def("itemExists", &MapMessage::itemExists)
        .def("getBoolean", &MapMessage::getBoolean)
        .def("setBoolean", &MapMessage::setBoolean)
        .def("getByte", &MapMessage::getByte)
        .def("setByte", &MapMessage::setByte)
        .def("getBytes", MapMessage_getBytes)
        .def("setBytes", &MapMessage::setBytes)
        .def("getChar", &MapMessage::getChar)
        .def("setChar", &MapMessage::setChar)
        .def("getDouble", &MapMessage::getDouble)
        .def("setDouble", &MapMessage::setDouble)
        .def("getFloat", &MapMessage::getFloat)
        .def("setFloat", &MapMessage::setFloat)
        .def("getInt", &MapMessage::getInt)
        .def("setInt", &MapMessage::setInt)
        .def("getLong", &MapMessage::getLong)
        .def("setLong", &MapMessage::setLong)
        .def("getShort", &MapMessage::getShort)
        .def("setShort", &MapMessage::setShort)
        .def("getString", &MapMessage::getString)
        .def("setString", &MapMessage::setString)
        ;
}
