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
#include <boost/python/str.hpp>
#include <boost/python/manage_new_object.hpp>
#include <boost/python/dict.hpp>

namespace py = boost::python;

#include <cms/BytesMessage.h>

#include <iostream>

namespace py = boost::python;

using cms::BytesMessage;
using cms::Message;

static const char* BytesMessage_docstring =
    "A message used for sending a stream of uninterpreted bytes.";
static const char* BytesMessage_bodyBytes_docstring =
    "Return the bytes that are contained in this message. Users should copy this "
    "data into a user allocated buffer.\n\nCheck L{bodyLength} to determine the "
    "number of bytes to expect.";
static const char* BytesMessage_bodyLength_docstring =
    "Returns the number of bytes contained in the body of this message.";
static const char* BytesMessage_reset_docstring =
    "Puts the message body in read-only mode and repositions the stream of bytes "
    "to the beginning.";
static const char* BytesMessage_readBoolean_docstring =
    "Reads a boolean from the bytes message stream.";
static const char* BytesMessage_writeBoolean_docstring =
    "Writes a boolean to the bytes message stream as a 1-byte value.\n\n"
    "The value true is written as the value (byte)1; the value false is "
    "written as the value (byte)0.";
static const char* BytesMessage_readByte_docstring =
    "Reads a byte from the bytes message stream.";
static const char* BytesMessage_writeByte_docstring =
    "Writes a byte to the bytes message stream as a 1-byte value.";
static const char* BytesMessage_readBytes_docstring =
    "Reads a byte array from the bytes message stream.\n\nIf the length of the "
    "vector value is less than the number of bytes remaining to be read from "
    "the stream, the vector should be filled. A subsequent call reads the next "
    "increment, and so on.\n\nIf the number of bytes remaining in the stream "
    "is less than the length of vector value, the bytes should be read into the "
    "vector. The return value of the total number of bytes read will be less "
    "than the length of the vector, indicating that there are no more bytes "
    "left to be read from the stream. The next read of the stream returns -1.";
static const char* BytesMessage_writeBytes_docstring =
    "Writes a byte array to the bytes message stream using the vector size as "
    "the number of bytes to write.";
static const char* BytesMessage_readChar_docstring = "Reads a char from the bytes message stream.";
static const char* BytesMessage_writeChar_docstring = "Writes a char to the bytes message stream as a 1-byte value.";
static const char* BytesMessage_readFloat_docstring = "Reads a 32 bit float from the bytes message stream.";
static const char* BytesMessage_writeFloat_docstring = "Writes a float to the bytes message stream as a 4-byte value.";
static const char* BytesMessage_readDouble_docstring = "Reads a 64 bit double from the bytes message stream.";
static const char* BytesMessage_writeDouble_docstring = "Writes a double to the bytes message stream as a 8-byte value.";
static const char* BytesMessage_readShort_docstring = "Reads a 16 bit signed short from the bytes message stream.";
static const char* BytesMessage_writeShort_docstring = "Writes a signed short to the bytes message stream as a 2-byte value.";
static const char* BytesMessage_readUnsignedShort_docstring = "Reads a 16 bit unsigned short from the bytes message stream.";
static const char* BytesMessage_writeUnsignedShort_docstring = "Writes a unsigned short to the bytes message stream as a 2-byte value.";
static const char* BytesMessage_readInt_docstring = "Reads a 32 bit signed integer from the bytes message stream.";
static const char* BytesMessage_writeInt_docstring = "Writes a signed integer to the bytes message stream as a 4-byte value.";
static const char* BytesMessage_readLong_docstring = "Reads a 64 bit long from the bytes message stream.";
static const char* BytesMessage_writeLong_docstring = "Writes a long to the bytes message stream as a 8-byte value.";
static const char* BytesMessage_readString_docstring = "Reads an ASCII string from the bytes message stream.";
static const char* BytesMessage_writeString_docstring = "Writes an ASCII string to the bytes message stream.";
static const char* BytesMessage_readUTF_docstring = "Reads an UTF string from the bytes message stream.";
static const char* BytesMessage_writeUTF_docstring = "Writes an UTF string to the bytes message stream.";

static PyObject* BytesMessage_getBodyBytes(BytesMessage const& self)
{
    return py::incref(py::str(reinterpret_cast<const char*>(self.getBodyBytes()), self.getBodyLength()).ptr());
}

static void BytesMessage_setBodyBytes(BytesMessage& This, const std::string& buffer)
{
    This.setBodyBytes(reinterpret_cast<const unsigned char*>(buffer.c_str()), buffer.length());
}

static std::string BytesMessage_readBytes(BytesMessage& This)
{
#if 0
    // TODO make this buffer size configurable
    std::vector<unsigned char> buffer(This.getBodyLength());
    std::size_t length = This.readBytes(buffer);
#endif
    return "";
}

static void BytesMessage_writeBytes(BytesMessage& This, const std::string& buffer)
{
    std::vector<unsigned char> buffer2(buffer.size());
    for (std::size_t i = 0; i < buffer.size(); i++) {
        buffer2.push_back(buffer[i]);
    }
    This.writeBytes(buffer2);
}

static BytesMessage* BytesMessage_deepcopy(BytesMessage* This, py::dict memo)
{
    return This->clone();
}

void export_BytesMessage()
{
    py::class_<BytesMessage, py::bases<Message>, boost::noncopyable>("BytesMessage", BytesMessage_docstring, py::no_init)
        .add_property("bodyBytes", BytesMessage_getBodyBytes, BytesMessage_setBodyBytes, BytesMessage_bodyBytes_docstring)
        .add_property("bodyLength", &BytesMessage::getBodyLength, BytesMessage_bodyLength_docstring)
        .def("reset", &BytesMessage::reset, BytesMessage_reset_docstring)
        .def("readBoolean", &BytesMessage::readBoolean, BytesMessage_readBoolean_docstring)
        .def("writeBoolean", &BytesMessage::writeBoolean, BytesMessage_writeBoolean_docstring)
        .def("readByte", &BytesMessage::readByte, BytesMessage_readByte_docstring)
        .def("writeByte", &BytesMessage::writeByte, BytesMessage_writeByte_docstring)
        .def("readBytes", BytesMessage_readBytes, BytesMessage_readBytes_docstring)
        .def("writeBytes", BytesMessage_writeBytes, BytesMessage_writeBytes_docstring)
        .def("readChar", &BytesMessage::readChar, BytesMessage_readChar_docstring)
        .def("writeChar", &BytesMessage::writeChar, BytesMessage_writeChar_docstring)
        .def("readFloat", &BytesMessage::readFloat, BytesMessage_readFloat_docstring)
        .def("writeFloat", &BytesMessage::writeFloat, BytesMessage_writeFloat_docstring)
        .def("readDouble", &BytesMessage::readDouble, BytesMessage_readDouble_docstring)
        .def("writeDouble", &BytesMessage::writeDouble, BytesMessage_writeDouble_docstring)
        .def("readShort", &BytesMessage::readShort, BytesMessage_readShort_docstring)
        .def("writeShort", &BytesMessage::writeShort, BytesMessage_writeShort_docstring)
        .def("readUnsignedShort", &BytesMessage::readUnsignedShort, BytesMessage_readUnsignedShort_docstring)
        .def("writeUnsignedShort", &BytesMessage::writeUnsignedShort, BytesMessage_writeUnsignedShort_docstring)
        .def("readInt", &BytesMessage::readInt, BytesMessage_readInt_docstring)
        .def("writeInt", &BytesMessage::writeInt, BytesMessage_writeInt_docstring)
        .def("readLong", &BytesMessage::readLong, BytesMessage_readLong_docstring)
        .def("writeLong", &BytesMessage::writeLong, BytesMessage_writeLong_docstring)
        .def("readString", &BytesMessage::readString, BytesMessage_readString_docstring)
        .def("writeString", &BytesMessage::writeString, BytesMessage_writeString_docstring)
        .def("readUTF", &BytesMessage::readUTF, BytesMessage_readUTF_docstring)
        .def("writeUTF", &BytesMessage::writeUTF, BytesMessage_writeUTF_docstring)
        .def("__deepcopy__", BytesMessage_deepcopy, py::return_value_policy<py::manage_new_object>());
}
