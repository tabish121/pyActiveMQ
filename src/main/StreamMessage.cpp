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

#include <cms/StreamMessage.h>

namespace py = boost::python;

using cms::StreamMessage;
using cms::Message;

void export_StreamMessage()
{
    py::class_<StreamMessage, py::bases<Message>, boost::noncopyable>("StreamMessage", py::no_init)
        .def("readBoolean", &StreamMessage::readBoolean)
        .def("writeBoolean", &StreamMessage::writeBoolean)
        .def("readByte", &StreamMessage::readByte)
        .def("writeByte", &StreamMessage::writeByte)
#if 0
        .def("readBytes", &StreamMessage::readBytes)
        .def("writeBytes", &StreamMessage::writeBytes)
#endif
        .def("readChar", &StreamMessage::readChar)
        .def("writeChar", &StreamMessage::writeChar)
        .def("readFloat", &StreamMessage::readFloat)
        .def("writeFloat", &StreamMessage::writeFloat)
        .def("readDouble", &StreamMessage::readDouble)
        .def("writeDouble", &StreamMessage::writeDouble)
        .def("readShort", &StreamMessage::readShort)
        .def("writeShort", &StreamMessage::writeShort)
        .def("readUnsignedShort", &StreamMessage::readUnsignedShort)
        .def("writeUnsignedShort", &StreamMessage::writeUnsignedShort)
        .def("readInt", &StreamMessage::readInt)
        .def("writeInt", &StreamMessage::writeInt)
        .def("readLong", &StreamMessage::readLong)
        .def("writeLong", &StreamMessage::writeLong)
        .def("readString", &StreamMessage::readString)
        .def("writeString", &StreamMessage::writeString)
        ;
}
