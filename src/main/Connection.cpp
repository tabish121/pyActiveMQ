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
#include <cms/Connection.h>
#include <cms/ExceptionListener.h>

using namespace boost::python;
using cms::Connection;
using cms::Startable;
using cms::Stoppable;
using cms::Closeable;
using cms::Session;
using cms::ExceptionListener;

Session* (Connection::*Connection_createSession0)() =
    &Connection::createSession;
Session* (Connection::*Connection_createSession1)(Session::AcknowledgeMode) =
    &Connection::createSession;

void export_Connection()
{
    class_<Connection, bases<Startable, Stoppable, Closeable>, boost::noncopyable>("Connection", no_init)
        .add_property("clientID", &Connection::getClientID)
        .add_property("exceptionListener",
                      make_function(&Connection::getExceptionListener, return_internal_reference<>()),
                      make_function(&Connection::setExceptionListener, with_custodian_and_ward<1,2>()))
        .def("createSession",
             Connection_createSession0,
             return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createSession",
             Connection_createSession1,
             return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        ;
}
