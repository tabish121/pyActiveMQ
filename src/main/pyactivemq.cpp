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

// TODO map clones so that they work with Python's copy module

#include <boost/python.hpp>
#include <activemq/core/ActiveMQConnectionFactory.h>

#include <cms/ExceptionListener.h>

using namespace boost::python;
using namespace cms;
using activemq::core::ActiveMQConnectionFactory;

Connection* (ActiveMQConnectionFactory::*ActiveMQConnectionFactory_createConnection0)() =
    &ActiveMQConnectionFactory::createConnection;
Connection* (ActiveMQConnectionFactory::*ActiveMQConnectionFactory_createConnection3)(
    const std::string&, const std::string&, const std::string&) =
    &ActiveMQConnectionFactory::createConnection;

Session* (Connection::*Connection_createSession0)() = &Connection::createSession;

void (TextMessage::*TextMessage_setText)(const std::string&) = &TextMessage::setText;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Session_createConsumer_overloads, createConsumer, 1, 3)

void CMSException_translator(const CMSException& e) {
    PyErr_SetString(PyExc_UserWarning, e.getMessage().c_str());
}

struct ExceptionListenerWrap : ExceptionListener, wrapper<ExceptionListener>
{
    virtual void onException(const CMSException& ex)
    {
#if 0
        this->getOverride("onException")(ex);
#else
        call<void>(this->get_override("onException").ptr(), ex);
#endif
    }
};

struct MessageListenerWrap : MessageListener, wrapper<MessageConsumer>
{
    virtual void onMessage(const Message* message)
    {
#if 0
        this->getOverride("onMessage")(message);
#else
        call<void>(this->get_override("onMessage").ptr(), message);
#endif
    }
};

BOOST_PYTHON_MODULE(pyactivemq)
{
    register_exception_translator<CMSException>(CMSException_translator);

    class_<ActiveMQConnectionFactory>("ActiveMQConnectionFactory")
        .def(init<const std::string&, optional<const std::string&, const std::string&, const std::string&> >())

#if 0
        .def("createConnection", &ActiveMQConnectionFactory::createConnection,
             ActiveMQConnectionFactory_overloads(),
             return_value_policy<manage_new_object>())
#else
        .def("createConnection",
             ActiveMQConnectionFactory_createConnection0,
             return_value_policy<manage_new_object>())
        .def("createConnection",
             ActiveMQConnectionFactory_createConnection3,
             return_value_policy<manage_new_object>())
#endif

        // TODO these should be properties
        .def("setUsername", &ActiveMQConnectionFactory::setUsername)
        .def("getUsername", &ActiveMQConnectionFactory::getUsername,
             return_value_policy<return_by_value>())
#if 0
        .def("setPassword", &ActiveMQConnectionFactory::setPassword)
        .def("getPassword", &ActiveMQConnectionFactory::getPassword, return_internal_reference<>())
        .def("setBrokerURL", &ActiveMQConnectionFactory::setBrokerURL)
        .def("getBrokerURL", &ActiveMQConnectionFactory::getBrokerURL, return_internal_reference<>())
        .def("setClientId", &ActiveMQConnectionFactory::setClientId)
        .def("getClientId", &ActiveMQConnectionFactory::getClientId, return_internal_reference<>())
#endif

#if 0
        .add_property("brokerURL",
                      make_getter(&ActiveMQConnectionFactory::getBrokerURL,
                                  return_value_policy<return_by_value>()),
                      &ActiveMQConnectionFactory::setBrokerURL,
            "Broker URL")

#endif
        ;


    class_<Startable, boost::noncopyable>("Startable", no_init)
        .def("start", &Startable::start)
        ;

    class_<Stoppable, boost::noncopyable>("Stoppable", no_init)
        .def("stop", &Stoppable::stop)
        ;

    class_<Closeable, boost::noncopyable>("Closeable", no_init)
        .def("close", &Closeable::close)
        ;

    class_<Connection, bases<Startable, Stoppable, Closeable>, boost::noncopyable>("Connection", no_init)
        .add_property("clientId", &Connection::getClientId)
#if 0
        // TODO setter
        .add_property("exceptionListener",
                      make_getter(&Connection::getExceptionListener,
                                  return_value_policy<reference_existing_object>()))
#endif
        .def("createSession", Connection_createSession0, return_value_policy<manage_new_object>())
        ;

    class_<Session, boost::noncopyable>("Session", no_init)
        .def("commit", &Session::commit)
        .def("rollback", &Session::rollback)
#if 0
        .def("createConsumer",
             &Session::createConsumer,
             Session_createConsumer_overloads(
                 args("destination", "selector", "nolocal"),
                 "XXX docstring")
             [return_value_policy<manage_new_object>()])
#endif
        .def("createTopic", &Session::createTopic, return_value_policy<manage_new_object>())
        .def("createQueue", &Session::createQueue, return_value_policy<manage_new_object>())
        .def("createTemporaryTopic", &Session::createTemporaryTopic, return_value_policy<manage_new_object>())
        .def("createTemporaryQueue", &Session::createTemporaryQueue, return_value_policy<manage_new_object>())
        .def("createMessage", &Session::createMessage, return_value_policy<manage_new_object>())
#if 0
        .def("createTextMessage", &Session::createTextMessage, return_value_policy<manage_new_object>())
        .def("createBytesMessage", &Session::createBytesMessage, return_value_policy<manage_new_object>())
#endif
        .def("createMapMessage", &Session::createMapMessage, return_value_policy<manage_new_object>())
#if 0
        .add_property("acknowledgeMode", &Session::getAcknowlegdeMode)
#endif
        .add_property("transacted", &Session::isTransacted)
        ;

    enum_<Session::AcknowledgeMode>("AcknowledgeMode")
        .value("AUTO_ACKNOWLEDGE", Session::AUTO_ACKNOWLEDGE)
        .value("DUPS_OK_ACKNOWLEDGE", Session::DUPS_OK_ACKNOWLEDGE)
        .value("CLIENT_ACKNOWLEDGE", Session::CLIENT_ACKNOWLEDGE)
        .value("SESSION_TRANSACTED", Session::SESSION_TRANSACTED)
        ;

    class_<Topic, bases<Destination>, boost::noncopyable>("Topic", no_init)
        .add_property("topicName", &Topic::getTopicName)
        ;

    class_<Queue, bases<Destination>, boost::noncopyable>("Queue", no_init)
        .add_property("queueName", &Queue::getQueueName)
        ;

    class_<TemporaryTopic, bases<Destination>, boost::noncopyable>("TemporaryTopic", no_init)
        .add_property("topicName", &Topic::getTopicName)
        ;

    class_<TemporaryQueue, bases<Destination>, boost::noncopyable>("TemporaryQueue", no_init)
        .add_property("queueName", &Queue::getQueueName)
        ;

    class_<Destination, boost::noncopyable>("Destination", no_init)
        .add_property("destinationType", &Destination::getDestinationType)
        .def("__str__", &Destination::toString)
        .def("__repr__", &Destination::toString)
        .add_property("providerString", &Destination::toProviderString)
        ;


    enum_<Destination::DestinationType>("DestinationType")
        .value("TOPIC", Destination::TOPIC)
        .value("QUEUE", Destination::QUEUE)
        .value("TEMPORARY_TOPIC", Destination::TEMPORARY_TOPIC)
        .value("TEMPORARY_QUEUE", Destination::TEMPORARY_QUEUE)
        ;

#if 0
    enum_<DeliveryMode>("DeliveryMode")
        .value("PERSISTANT", DeliveryMode::PERSISTANT)
        .value("NON_PERSISTANT", DeliveryMode::NON_PERSISTANT)
        ;
#endif
#if 0
    DeliveryMode_scope = class_<DeliveryMode>("DeliveryMode");
    DeliveryMode_scope.attr("PERSISTANT") = DeliveryMode::PERSISTANT;
    DeliveryMode_scope.attr("NON_PERSISTANT") = DeliveryMode::NON_PERSISTANT;
#endif

    class_<Message, boost::noncopyable>("Message", no_init)
        ;

    class_<TextMessage, bases<Message>, boost::noncopyable>("TextMessage", no_init)
#if 0
        .def_readwrite("text", &TextMessage::getText, TextMessage_setText)
#endif
        ;

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

    // TODO see if we can do something to make MapMessage work nicely
    // with python maps
    class_<MapMessage, bases<Message>, boost::noncopyable>("MapMessage", no_init)
        ;

    class_<ExceptionListenerWrap, boost::noncopyable>("ExceptionListener", no_init)
        .def("onException", pure_virtual(&ExceptionListener::onException))
        ;

    class_<MessageProducer, bases<Closeable>, boost::noncopyable>("MessageProducer", no_init)
        ;

    class_<MessageListenerWrap, boost::noncopyable>("MessageListener", no_init)
        .def("onMessage", pure_virtual(&MessageListener::onMessage))
        ;

    class_<MessageConsumer, bases<Closeable>, boost::noncopyable>("MessageConsumer", no_init)
        // TODO make messageListener a property
        // TODO 2 receive signatures
        .def("receiveNoWait",
             &MessageConsumer::receiveNoWait,
             return_value_policy<manage_new_object>())
#if 0
        .def_readonly("messageSelector", &MessageConsumer::getMessageSelector)
#else
        .add_property("messageSelector", &MessageConsumer::getMessageSelector)
#endif
        ;
}
