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

#include <cms/MapMessage.h>

namespace py = boost::python;

using cms::MapMessage;
using cms::Message;

static const char* MapMessage_docstring =
    "A C{MapMessage} object is used to send a set of name-value pairs.\n\n"
    "The names are C{String} objects, and the values are primitive data types in the "
    "Java programming language. The names must have a value that is not null, and "
    "not an empty string. The entries can be accessed sequentially or randomly by "
    "name. The order of the entries is undefined. C{MapMessage} inherits from the "
    "L{Message} interface and adds a message body that contains a C{Map}.\n\n"
    "When a client receives a C{MapMessage}, it is in read-only mode. If a client "
    "attempts to write to the message at this point, a L{CMSException} is thrown.";
static const char* MapMessage_mapNames_docstring = "Returns an enumeration of all the names in the C{MapMessage} object.";
static const char* MapMessage_itemExists_docstring = "Indicates whether an item exists in this C{MapMessage} object.";
static const char* MapMessage_getBoolean_docstring = "Returns the C{Boolean} value of the specified name.";
static const char* MapMessage_setBoolean_docstring = "Sets a boolean value with the specified name into the C{Map}.";
static const char* MapMessage_getByte_docstring = "Returns the byte value of the specified name.";
static const char* MapMessage_setByte_docstring = "Sets a byte value with the specified name into the C{Map}.";
static const char* MapMessage_getBytes_docstring = "Returns the bytes value of the specified name.";
static const char* MapMessage_setBytes_docstring = "Sets a bytes value with the specified name into the C{Map}.";
static const char* MapMessage_getChar_docstring = "Returns the char value of the specified name.";
static const char* MapMessage_setChar_docstring = "Sets a char value with the specified name into the C{Map}.";
static const char* MapMessage_getDouble_docstring = "Returns the double value of the specified name.";
static const char* MapMessage_setDouble_docstring = "Sets a double value with the specified name into the C{Map}.";
static const char* MapMessage_getFloat_docstring = "Returns the float value of the specified name.";
static const char* MapMessage_setFloat_docstring = "Sets a float value with the specified name into the C{Map}.";
static const char* MapMessage_getInt_docstring = "Returns the int value of the specified name.";
static const char* MapMessage_setInt_docstring = "Sets a int value with the specified name into the C{Map}.";
static const char* MapMessage_getLong_docstring = "Returns the long value of the specified name.";
static const char* MapMessage_setLong_docstring = "Sets a long value with the specified name into the C{Map}.";
static const char* MapMessage_getShort_docstring = "Returns the short value of the specified name.";
static const char* MapMessage_setShort_docstring = "Sets a short value with the specified name into the C{Map}.";
static const char* MapMessage_getString_docstring = "Returns the string value of the specified name.";
static const char* MapMessage_setString_docstring = "Sets a String value with the specified name into the C{Map}.";

static const char* MapMessage_getBytes(MapMessage& This, const std::string& name)
{
#if 0
    return reinterpret_cast<const char*>(This.getBytes(name));
#else
    return 0;
#endif
}

static MapMessage* MapMessage_deepcopy(MapMessage* This, py::dict memo)
{
    return dynamic_cast<MapMessage*>(This->clone());
}

void export_MapMessage()
{
    py::class_<MapMessage, py::bases<Message>, boost::noncopyable>("MapMessage", MapMessage_docstring, py::no_init)
        .add_property("mapNames", &MapMessage::getMapNames, MapMessage_mapNames_docstring)
        .def("itemExists", &MapMessage::itemExists, MapMessage_itemExists_docstring)
        .def("getBoolean", &MapMessage::getBoolean, MapMessage_getBoolean_docstring)
        .def("setBoolean", &MapMessage::setBoolean, MapMessage_setBoolean_docstring)
        .def("getByte", &MapMessage::getByte, MapMessage_getByte_docstring)
        .def("setByte", &MapMessage::setByte, MapMessage_setByte_docstring)
        .def("getBytes", MapMessage_getBytes, MapMessage_getBytes_docstring)
        .def("setBytes", &MapMessage::setBytes, MapMessage_setBytes_docstring)
        .def("getChar", &MapMessage::getChar, MapMessage_getChar_docstring)
        .def("setChar", &MapMessage::setChar, MapMessage_setChar_docstring)
        .def("getDouble", &MapMessage::getDouble, MapMessage_getDouble_docstring)
        .def("setDouble", &MapMessage::setDouble, MapMessage_setDouble_docstring)
        .def("getFloat", &MapMessage::getFloat, MapMessage_getFloat_docstring)
        .def("setFloat", &MapMessage::setFloat, MapMessage_setFloat_docstring)
        .def("getInt", &MapMessage::getInt, MapMessage_getInt_docstring)
        .def("setInt", &MapMessage::setInt, MapMessage_setInt_docstring)
        .def("getLong", &MapMessage::getLong, MapMessage_getLong_docstring)
        .def("setLong", &MapMessage::setLong, MapMessage_setLong_docstring)
        .def("getShort", &MapMessage::getShort, MapMessage_getShort_docstring)
        .def("setShort", &MapMessage::setShort, MapMessage_setShort_docstring)
        .def("getString", &MapMessage::getString, MapMessage_getString_docstring)
        .def("setString", &MapMessage::setString, MapMessage_setString_docstring)
        .def("__deepcopy__", MapMessage_deepcopy, py::return_value_policy<py::manage_new_object>());
}
