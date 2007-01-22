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

std::string BytesMessage_getBodyBytes(BytesMessage const& This)
{
    // convert body bytes into a null-terminated buffer
    char* buffer = new char[This.getBodyLength() + 1];
    memcpy(buffer, This.getBodyBytes(), This.getBodyLength());
    buffer[This.getBodyLength()] = '\0';
    std::string buffer2(buffer);
    delete[] buffer;
    buffer = 0;
    return buffer2;
}

void BytesMessage_setBodyBytes(BytesMessage& This, const std::string& buffer)
{
    This.setBodyBytes(reinterpret_cast<const unsigned char*>(buffer.c_str()), buffer.length());
}

std::string BytesMessage_readBytes(BytesMessage& This)
{
    std::vector<unsigned char> buffer;
    int length = This.readBytes(buffer);
    return "";
}

void BytesMessage_writeBytes(BytesMessage& This, const std::string& buffer)
{
    std::vector<unsigned char> buffer2(buffer.size());
    for (std::size_t i = 0; i < buffer.size(); i++)
    {
        buffer2.push_back(buffer[i]);
    }
    This.writeBytes(buffer2);
}

void export_BytesMessage()
{
    class_<BytesMessage, bases<Message>, boost::noncopyable>("BytesMessage", no_init)
        .add_property("bodyBytes", BytesMessage_getBodyBytes, BytesMessage_setBodyBytes)
        .add_property("bodyLength", &BytesMessage::getBodyLength)
        .def("reset", &BytesMessage::reset)
        .def("readBoolean", &BytesMessage::readBoolean)
        .def("writeBoolean", &BytesMessage::writeBoolean)
        .def("readByte", &BytesMessage::readByte)
        .def("writeByte", &BytesMessage::writeByte)
        .def("readBytes", BytesMessage_readBytes)
        .def("writeBytes", BytesMessage_writeBytes)
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
        ;
}
