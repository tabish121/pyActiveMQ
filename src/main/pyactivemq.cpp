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
Session* (Connection::*Connection_createSession1)(Session::AcknowledgeMode) = &Connection::createSession;

MessageConsumer* (Session::*Session_createConsumer1)(const Destination*) = &Session::createConsumer;
MessageConsumer* (Session::*Session_createConsumer2)(const Destination*, const std::string&) = &Session::createConsumer;
MessageConsumer* (Session::*Session_createConsumer3)(const Destination*, const std::string&, bool) = &Session::createConsumer;

TextMessage* (Session::*Session_createTextMessage0)() = &Session::createTextMessage;
TextMessage* (Session::*Session_createTextMessage1)(const std::string&) = &Session::createTextMessage;

BytesMessage* (Session::*Session_createBytesMessage0)() = &Session::createBytesMessage;
// TODO 2-argument createBytesMessage

Message* (MessageConsumer::*MessageConsumer_receive0)() = &MessageConsumer::receive;
Message* (MessageConsumer::*MessageConsumer_receive1)(int) = &MessageConsumer::receive;

void (MessageProducer::*MessageProducer_send1)(Message*) = &MessageProducer::send;
void (MessageProducer::*MessageProducer_send4)(Message*, int, int, long long) = &MessageProducer::send;
void (MessageProducer::*MessageProducer_send2)(const Destination*, Message*) = &MessageProducer::send;
void (MessageProducer::*MessageProducer_send5)(const Destination*, Message*, int, int, long long) = &MessageProducer::send;

void (TextMessage::*TextMessage_setText)(const std::string&) = &TextMessage::setText;

void CMSException_translator(const CMSException& e) {
    PyErr_SetString(PyExc_UserWarning, e.getMessage().c_str());
}

struct ExceptionListenerWrap : ExceptionListener, wrapper<ExceptionListener>
{
    virtual void onException(const CMSException& ex)
    {
        // should be: this->getOverride("onException")(ex);
        // but that doesn't work with Visual C++
        call<void>(this->get_override("onException").ptr(), ex);
    }
};

struct MessageListenerWrap : MessageListener, wrapper<MessageConsumer>
{
    virtual void onMessage(const Message* message)
    {
        // should be: this->getOverride("onMessage")(message);
        // but that doesn't work with Visual C++
        call<void>(this->get_override("onMessage").ptr(), message);
    }
};

template<typename T>
struct vector_to_list
{
    static PyObject* convert(const std::vector<std::string>& x)
    {
        list y;
        for (std::vector<T>::const_iterator it = x.begin();
             it != x.end();
             ++it)
        {
             y.append(*it);
        }
        return y.ptr();
    }
};

BOOST_PYTHON_MODULE(pyactivemq)
{
    register_exception_translator<CMSException>(CMSException_translator);
    to_python_converter<std::vector<std::string>, vector_to_list<std::string> >();

    class_<ActiveMQConnectionFactory>("ActiveMQConnectionFactory")
        .def(init<const std::string&, optional<const std::string&, const std::string&, const std::string&> >())
        .def("createConnection",
             ActiveMQConnectionFactory_createConnection0,
             return_value_policy<manage_new_object>())
        .def("createConnection",
             ActiveMQConnectionFactory_createConnection3,
             return_value_policy<manage_new_object>())
#if 0
        .add_property("username",
                      make_getter(&ActiveMQConnectionFactory::getUsername,
                                  return_value_policy<return_by_value>()),
                      &ActiveMQConnectionFactory::setPassword)
        .add_property("password",
                      make_getter(&ActiveMQConnectionFactory::getPassword,
                                  return_value_policy<return_by_value>()),
                      &ActiveMQConnectionFactory::setPassword)
        .add_property("brokerURL",
                      make_getter(&ActiveMQConnectionFactory::getBrokerURL,
                                  return_value_policy<return_by_value>()),
                      &ActiveMQConnectionFactory::setBrokerURL)
        .add_property("clientId",
                      make_getter(&ActiveMQConnectionFactory::getClientId,
                                  return_value_policy<return_by_value>()),
                      &ActiveMQConnectionFactory::setClientId)
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
        .def("createSession",
             Connection_createSession0,
             return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createSession",
             Connection_createSession1,
             return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        ;

    // TODO need to keep a reference to Session while stuff it created
    // is alive, especially producers and consumers
    // TODO probably need with_custodian_and_ward_postcall in a few places
    // http://mail.python.org/pipermail/c++-sig/2004-November/008189.html
    class_<Session, bases<Closeable>, boost::noncopyable>("Session", no_init)
        .def("commit", &Session::commit)
        .def("rollback", &Session::rollback)
        .def("createConsumer", Session_createConsumer1, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createConsumer", Session_createConsumer2, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createConsumer", Session_createConsumer3, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createDurableConsumer", &Session::createDurableConsumer, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createProducer", &Session::createProducer, return_value_policy<manage_new_object, with_custodian_and_ward_postcall<0, 1> >())
        .def("createTopic", &Session::createTopic, return_value_policy<manage_new_object>())
        .def("createQueue", &Session::createQueue, return_value_policy<manage_new_object>())
        .def("createTemporaryTopic", &Session::createTemporaryTopic, return_value_policy<manage_new_object>())
        .def("createTemporaryQueue", &Session::createTemporaryQueue, return_value_policy<manage_new_object>())
        .def("createMessage", &Session::createMessage, return_value_policy<manage_new_object>())
        .def("createTextMessage", Session_createTextMessage0, return_value_policy<manage_new_object>())
        .def("createTextMessage", Session_createTextMessage1, return_value_policy<manage_new_object>())
        .def("createBytesMessage", Session_createBytesMessage0, return_value_policy<manage_new_object>())
        .def("createMapMessage", &Session::createMapMessage, return_value_policy<manage_new_object>())
        .add_property("acknowledgeMode", &Session::getAcknowledgeMode)
        .add_property("transacted", &Session::isTransacted)
        ;

    enum_<Session::AcknowledgeMode>("AcknowledgeMode")
        .value("AUTO_ACKNOWLEDGE", Session::AUTO_ACKNOWLEDGE)
        .value("DUPS_OK_ACKNOWLEDGE", Session::DUPS_OK_ACKNOWLEDGE)
        .value("CLIENT_ACKNOWLEDGE", Session::CLIENT_ACKNOWLEDGE)
        .value("SESSION_TRANSACTED", Session::SESSION_TRANSACTED)
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

    // TODO maybe some of these properties should be read-only?
    class_<Message, boost::noncopyable>("Message", no_init)
        // TODO clone?
        .def("acknowledge", &Message::acknowledge)
        .def("clearBody", &Message::clearBody)
        .def("clearProperties", &Message::clearProperties)
        .add_property("propertyNames", &Message::getPropertyNames)
        .def("propertyExists", &Message::propertyExists)
        .def("getBooleanProperty", &Message::getBooleanProperty)
        .def("getByteProperty", &Message::getByteProperty)
        .def("getDoubleProperty", &Message::getDoubleProperty)
        .def("getFloatProperty", &Message::getFloatProperty)
        .def("getIntProperty", &Message::getIntProperty)
        .def("getLongProperty", &Message::getLongProperty)
        .def("getShortProperty", &Message::getShortProperty)
        .def("getStringProperty", &Message::getStringProperty)
        .def("setBooleanProperty", &Message::setBooleanProperty)
        .def("setByteProperty", &Message::setByteProperty)
        .def("setDoubleProperty", &Message::setDoubleProperty)
        .def("setFloatProperty", &Message::setFloatProperty)
        .def("setIntProperty", &Message::setIntProperty)
        .def("setLongProperty", &Message::setLongProperty)
        .def("setShortProperty", &Message::setShortProperty)
        .def("setStringProperty", &Message::setStringProperty)
        .add_property("correlationId", &Message::getCMSCorrelationId, &Message::setCMSCorrelationId)
        .add_property("deliveryMode", &Message::getCMSDeliveryMode, &Message::setCMSDeliveryMode)
#if 0
        .add_property("destination", &Message::getCMSDestination, &Message::setCMSDestination)
#endif
        .add_property("expiration", &Message::getCMSExpiration, &Message::setCMSExpiration)
        .add_property("messageId", &Message::getCMSMessageId, &Message::setCMSMessageId)
        .add_property("priority", &Message::getCMSPriority, &Message::setCMSPriority)
        .add_property("redelivered", &Message::getCMSRedelivered, &Message::setCMSRedelivered)
#if 0
        .add_property("replyTo", &Message::getCMSReplyTo, &Message::setCMSReplyTo)
#endif
        .add_property("timeStamp", &Message::getCMSTimeStamp, &Message::setCMSTimeStamp)
        .add_property("messageType", &Message::getCMSMessageType, &Message::setCMSMessageType)
        ;

    class_<TextMessage, bases<Message>, boost::noncopyable>("TextMessage", no_init)
        .add_property("text", &TextMessage::getText, TextMessage_setText)
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
        // TODO getMapNames
        .def("itemExists", &MapMessage::itemExists)
        .def("getBoolean", &MapMessage::getBoolean)
        .def("setBoolean", &MapMessage::setBoolean)
        .def("getByte", &MapMessage::getByte)
        .def("setByte", &MapMessage::setByte)
#if 0
        // TODO need a return value policy
        .def("getBytes", &MapMessage::getBytes)
#endif
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

    class_<ExceptionListenerWrap, boost::noncopyable>("ExceptionListener")
        .def("onException", pure_virtual(&ExceptionListener::onException))
        ;

    class_<MessageProducer, bases<Closeable>, boost::noncopyable>("MessageProducer", no_init)
        .def("send", MessageProducer_send1)
        .def("send", MessageProducer_send4)
        .def("send", MessageProducer_send2)
        .def("send", MessageProducer_send5)
        .add_property("deliveryMode",
                      &MessageProducer::getDeliveryMode,
                      &MessageProducer::setDeliveryMode)
        .add_property("disableMessageId",
                      &MessageProducer::getDisableMessageId,
                      &MessageProducer::setDisableMessageId)
        .add_property("disableMessageTimeStamp",
                      &MessageProducer::getDisableMessageTimeStamp,
                      &MessageProducer::setDisableMessageTimeStamp)
        .add_property("priority",
                      &MessageProducer::getPriority,
                      &MessageProducer::setPriority)
        .add_property("timeToLive",
                      &MessageProducer::getTimeToLive,
                      &MessageProducer::setTimeToLive)
        ;

    // TODO look at issues with the GIL here when multiple session
    // threads call into the Python code (could happen when Openwire
    // support is added)
    class_<MessageListenerWrap, boost::noncopyable>("MessageListener")
        .def("onMessage", pure_virtual(&MessageListener::onMessage))
        ;

    class_<MessageConsumer, bases<Closeable>, boost::noncopyable>("MessageConsumer", no_init)
        .def("receive", MessageConsumer_receive0, return_value_policy<manage_new_object>())
        .def("receive", MessageConsumer_receive1, return_value_policy<manage_new_object>())
        .def("receiveNoWait",
             &MessageConsumer::receiveNoWait,
             return_value_policy<manage_new_object>())
#if 0
        .add_property("messageListener",
                      &MessageConsumer::getMessageListener,
                      &MessageConsumer::setMessageListener)
#endif
        .add_property("messageSelector", &MessageConsumer::getMessageSelector)
        ;
}
