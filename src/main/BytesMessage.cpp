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
#include <cms/BytesMessage.h>

using namespace boost::python;
using cms::BytesMessage;
using cms::Message;

void export_BytesMessage()
{
    class_<BytesMessage, bases<Message>, boost::noncopyable>("BytesMessage", no_init)
        // TODO setBodyBytes -- do something with buffer protocol?
        // TODO getBodyBytes
        .add_property("bodyLength", &BytesMessage::getBodyLength)
        .def("reset", &BytesMessage::reset)
        .def("readBoolean", &BytesMessage::readBoolean)
        .def("writeBoolean", &BytesMessage::writeBoolean)
        .def("readByte", &BytesMessage::readByte)
        .def("writeByte", &BytesMessage::writeByte)
        // TODO readBytes
        // TODO writeBytes
        .def("readChar", &BytesMessage::readChar)
        .def("writeChar", &BytesMessage::writeChar)
        .def("readFloat", &BytesMessage::readFloat)
        .def("writeFloat", &BytesMessage::writeFloat)
        .def("readDouble", &BytesMessage::readDouble)
        .def("writeDouble", &BytesMessage::writeDouble)
        .def("readShort", &BytesMessage::readShort)
        .def("writeShort", &BytesMessage::writeShort)
        .def("readUnsignedShort", &BytesMessage::readUnsignedShort)
        .def("writeUnsignedShort", &BytesMessage::writeUnsignedShort)
        .def("readInt", &BytesMessage::readInt)
        .def("writeInt", &BytesMessage::writeInt)
        .def("readLong", &BytesMessage::readLong)
        .def("writeLong", &BytesMessage::writeLong)
        .def("readString", &BytesMessage::readString)
        .def("writeString", &BytesMessage::writeString)
        .def("readUTF", &BytesMessage::readUTF)
        .def("writeUTF", &BytesMessage::writeUTF)
        // TODO clone?
        ;
}
