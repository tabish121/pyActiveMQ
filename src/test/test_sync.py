# Copyright 2007 Albert Strasheim <fullung@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from util import *
set_local_path()
import pyactivemq
restore_path()

import sys

class _test_sync:
    random_topic = random_topic
    random_queue = random_queue

    def assertAttrReadOnly(self, obj, attrname):
        self.assert_(hasattr(obj, attrname))
        try:
            setattr(obj, attrname, None)
            self.assert_(False, '%s attribute must be read-only' % attrname)
        except AttributeError:
            pass

    def test_Connection(self):
        self.assertEqual(2, sys.getrefcount(self.conn))
        conn = self.conn
        self.assertEqual(3, sys.getrefcount(conn))
        self.assert_(isinstance(conn, pyactivemq.Closeable))
        self.assert_(isinstance(conn, pyactivemq.Connection))
        AcknowledgeMode = pyactivemq.AcknowledgeMode
        session0 = conn.createSession()
        session1 = conn.createSession(AcknowledgeMode.AUTO_ACKNOWLEDGE)
        session2 = conn.createSession(AcknowledgeMode.DUPS_OK_ACKNOWLEDGE)
        session3 = conn.createSession(AcknowledgeMode.CLIENT_ACKNOWLEDGE)
        session4 = conn.createSession(AcknowledgeMode.SESSION_TRANSACTED)
        self.assertEqual(8, sys.getrefcount(conn))
        self.assert_(conn.clientID is not None)
        conn.close()
        self.assertEqual('', conn.clientID)
        del conn, session0, session1, session2, session3, session4
        self.assertEqual(2, sys.getrefcount(self.conn))

    def test_Connection_ExceptionListener(self):
        conn = self.conn
        self.assert_(conn.exceptionListener is None)
        class ExceptionListener(pyactivemq.ExceptionListener):
            def onException(ex):
                pass
        exlistener = ExceptionListener()
        self.assertEqual(2, sys.getrefcount(exlistener))
        conn.exceptionListener = exlistener
        self.assertEqual(3, sys.getrefcount(exlistener))
        self.assert_(exlistener is conn.exceptionListener)
        del exlistener
        self.assertEqual(2, sys.getrefcount(conn.exceptionListener))
        conn.exceptionListener = None

    def test_Session(self):
        session = self.conn.createSession()
        self.assert_(isinstance(session, pyactivemq.Closeable))
        self.assert_(isinstance(session, pyactivemq.Session))
        self.assert_(not session.transacted)
        AcknowledgeMode = pyactivemq.AcknowledgeMode
        ackmode = AcknowledgeMode.AUTO_ACKNOWLEDGE
        self.assertEqual(ackmode, session.acknowledgeMode)
        self.assertAttrReadOnly(session, 'acknowledgeMode')
        session.close()

    def test_Topic_and_Queue(self):
        DestinationType = pyactivemq.DestinationType
        session = self.conn.createSession()
        topic = session.createTopic("topic")
        self.assertEqual("topic", topic.name)
        self.assertEqual(DestinationType.TOPIC, topic.destinationType)
        self.assert_(isinstance(topic, pyactivemq.Destination))
        topic2 = session.createTopic("topic2")
        queue = session.createQueue("queue")
        self.assertEqual("queue", queue.name)
        self.assertEqual(DestinationType.QUEUE, queue.destinationType)
        self.assert_(isinstance(queue, pyactivemq.Destination))
        queue2 = session.createQueue("queue2")
        self.assertEqual(topic, topic)
        self.assertNotEqual(topic, topic2)
        self.assertEqual(topic.destinationType, topic2.destinationType)
        self.assertNotEqual(topic, queue)
        self.assertEqual(queue, queue)
        self.assertNotEqual(queue, queue2)
        self.assertEqual(queue.destinationType, queue2.destinationType)
        self.assertNotEqual(queue, topic)

    def test_MessageConsumer(self):
        session = self.conn.createSession()
        self.assertEqual(2, sys.getrefcount(session))
        topic = session.createTopic("topic")
        self.assertEqual(2, sys.getrefcount(session))
        consumer1 = session.createConsumer(topic)
        self.assertEqual(3, sys.getrefcount(session))
        self.assert_(isinstance(consumer1, pyactivemq.Closeable))
        self.assert_(isinstance(consumer1, pyactivemq.MessageConsumer))
        consumer2 = session.createConsumer(topic, "select1")
        self.assertEqual(4, sys.getrefcount(session))
        self.assertEqual('select1', consumer2.messageSelector)
        try:
            consumer2.messageSelector = 'select2'
            # can't set message selector after consumer creation
            self.assert_(False, 'Expected AttributeError to be raised')
        except AttributeError:
            pass
        consumer3 = session.createConsumer(topic, "", True)
        self.assertEqual(5, sys.getrefcount(session))
        del consumer1, consumer2, consumer3
        self.assertEqual(2, sys.getrefcount(session))

    def test_MessageProducer(self):
        session = self.conn.createSession()
        self.assertEqual(2, sys.getrefcount(session))
        topic = session.createTopic("topic")
        self.assertEqual(2, sys.getrefcount(session))
        producer = session.createProducer(topic)
        self.assertEqual(3, sys.getrefcount(session))
        from pyactivemq import DeliveryMode
        self.assertEqual(DeliveryMode.PERSISTENT, producer.deliveryMode)
        producer.deliveryMode = DeliveryMode.NON_PERSISTENT
        self.assertEqual(DeliveryMode.NON_PERSISTENT, producer.deliveryMode)
        self.assertEqual(False, producer.disableMessageID)
        producer.disableMessageID = True
        self.assertEqual(True, producer.disableMessageID)
        self.assertEqual(False, producer.disableMessageTimeStamp)
        producer.disableMessageTimeStamp = True
        self.assertEqual(True, producer.disableMessageTimeStamp)
        # time to live of 0 means message never expires
        self.assertEqual(0, producer.timeToLive)
        # unit of time to live is milliseconds
        producer.timeToLive = 60
        self.assertEqual(60, producer.timeToLive)
        del producer
        self.assertEqual(2, sys.getrefcount(session))

    def _check_Message_properties(self, msg):
        self.assertEqual(0, len(msg.propertyNames))

        properties = {
            'bool1' : (True, msg.getBooleanProperty, msg.setBooleanProperty),
            'byte1' : (123, msg.getByteProperty, msg.setByteProperty),
            'double1' : (123.456, msg.getDoubleProperty, msg.setDoubleProperty),
            'float1' : (123.456, msg.getDoubleProperty, msg.setDoubleProperty),
            'int1' : (123456, msg.getIntProperty, msg.setIntProperty),
            'long1' : (123456789, msg.getLongProperty, msg.setLongProperty),
            'short1' : (1234, msg.getShortProperty, msg.setShortProperty),
            'string1' : ('hello123', msg.getStringProperty, msg.setStringProperty)
            }

        for name, (value, getter, setter) in properties.iteritems():
            self.assert_(not msg.propertyExists(name))
            setter(name, value)
            self.assert_(msg.propertyExists(name))
            newvalue = getter(name)
            if name in ['float1', 'double1']:
                self.assertAlmostEqual(value, newvalue, 5)
            else:
                self.assertEqual(value, newvalue)

        self.assertEqual(len(properties), len(msg.propertyNames))
        msg.clearProperties()
        self.assertEqual(0, len(msg.propertyNames))

        self.assertEqual('', msg.correlationID)
        msg.correlationID = 'corrid'
        self.assertEqual('corrid', msg.correlationID)
        self.assertEqual('', msg.type)
        msg.type = 'type'
        self.assertEqual('type', msg.type)

        self.assertAttrReadOnly(msg, 'deliveryMode')
        self.assertAttrReadOnly(msg, 'expiration')
        self.assertAttrReadOnly(msg, 'messageID')
        self.assertAttrReadOnly(msg, 'priority')
        self.assertAttrReadOnly(msg, 'redelivered')
        self.assertAttrReadOnly(msg, 'timestamp')

    def test_Message(self):
        session = self.conn.createSession()
        message = session.createMessage()
        self.assert_(isinstance(message, pyactivemq.Message))
        self.assert_(message.destination is None)
        self.assert_(message.replyTo is None)
        self._check_Message_properties(message)

    def test_TextMessage(self):
        session = self.conn.createSession()
        textMessage = session.createTextMessage()
        self.assert_(isinstance(textMessage, pyactivemq.Message))
        self.assert_(isinstance(textMessage, pyactivemq.TextMessage))
        self._check_Message_properties(textMessage)

        textMessage.text = "bye"
        self.assertEqual("bye", textMessage.text)
        textMessage = session.createTextMessage("hello")
        self.assertEqual("hello", textMessage.text)

        queue = session.createQueue("queue")
        self.assertEqual(2, sys.getrefcount(queue))
        textMessage.replyTo = queue
        self.assertEqual(3, sys.getrefcount(queue))
        self.assertEqual(queue, textMessage.replyTo)
        del queue
        self.assertEqual(1, sys.getrefcount(textMessage.replyTo))
        # TODO allow derived type of destination to be retrieved from
        # replyTo and destination properties
        #self.assert_(isinstance(textMessage.replyTo, pyactivemq.Queue))

    def test_BytesMessage(self):
        session = self.conn.createSession()
        bytesMessage = session.createBytesMessage()
        self.assert_(isinstance(bytesMessage, pyactivemq.Message))
        self.assert_(isinstance(bytesMessage, pyactivemq.BytesMessage))
        self._check_Message_properties(bytesMessage)

        self.assertEqual(0, bytesMessage.bodyLength)
        bytesMessage.bodyBytes = 'hello123'
        self.assertEqual('hello123', bytesMessage.bodyBytes)
        self.assert_(bytesMessage.replyTo is None)
        queue = session.createQueue("queue")
        bytesMessage.replyTo = queue
        self.assertEqual(queue, bytesMessage.replyTo)

    def test_send_Message(self):
        session = self.conn.createSession()
        message = session.createMessage()
        queue = self.random_queue(session)
        consumer = session.createConsumer(queue)
        self.assert_(consumer.messageListener is None)
        producer = session.createProducer(queue)
        self.conn.start()
        producer.send(message)
        msg = consumer.receive(timeout=5000)
        self.assert_(msg is not None)
        self.assert_(isinstance(msg, pyactivemq.Message))
        self.assertEqual(str(msg.destination), str(queue))
        self.assertEqual(queue, msg.destination)
        msg = consumer.receive(50)
        self.assert_(msg is None)

    def test_send_TextMessage(self):
        session = self.conn.createSession()

        textMessage = session.createTextMessage()
        queue = session.createQueue("queue")
        textMessage.replyTo = queue

        topic = self.random_topic(session)
        consumer = session.createConsumer(topic)
        self.assert_(consumer.messageListener is None)
        producer = session.createProducer(topic)
        del session

        self.conn.start()
        del self.conn
        producer.send(textMessage)
        msg = consumer.receive(5000)

        self.assert_(msg is not None)
        self.assert_(isinstance(msg, pyactivemq.Message))
        self.assert_(isinstance(msg, pyactivemq.TextMessage))
        self.assertEqual(str(msg.destination), str(topic))
        self.assertEqual(topic, msg.destination)
        self.assertEqual(str(queue), str(msg.replyTo))
        self.assertEqual(queue, msg.replyTo)

        msg = consumer.receive(50)
        self.assert_(msg is None)

    def test_send_BytesMessage(self):
        session = self.conn.createSession()
        topic = self.random_topic(session)
        consumer = session.createConsumer(topic)
        producer = session.createProducer(topic)
        bytesMessage = session.createBytesMessage()
        bytesMessage.bodyBytes = 'hello123'
        bytesMessage.replyTo = topic

        self.conn.start()
        producer.send(bytesMessage)
        msg = consumer.receive(5000)

        self.assert_(msg is not None)
        self.assert_(isinstance(msg, pyactivemq.Message))
        self.assert_(isinstance(msg, pyactivemq.BytesMessage))
        self.assertEqual('hello123', msg.bodyBytes)
        self.assertEqual(topic, msg.destination)
        self.assertEqual(str(topic), str(msg.destination))
        self.assertEqual(str(topic), str(msg.replyTo))
        self.assertEqual(topic, msg.replyTo)

        bytesMessage = session.createBytesMessage()
        bytesMessage.writeBytes('hello123')
        producer.send(bytesMessage)
        msg = consumer.receive(5000)
        self.assert_(msg is not None)
        # XXX this doesn't return anything yet
        #self.assertEqual('hello123', msg.readBytes())
        self.assertEqual('', msg.readBytes())

        msg = consumer.receive(50)
        self.assert_(msg is None)

    def test_BytesMessage_bodyBytes(self):
        session = self.conn.createSession()
        topic = self.random_topic(session)
        consumer = session.createConsumer(topic)
        producer = session.createProducer(topic)
        del topic
        bytesMessage = session.createBytesMessage()
        bytesMessage.bodyBytes = '\x00\x00\x00'
        self.assertEqual(3, bytesMessage.bodyLength)

        self.conn.start()
        producer.send(bytesMessage)
        msg = consumer.receive(5000)

        self.assert_(msg is not None)
        self.assert_(isinstance(msg, pyactivemq.Message))
        self.assert_(isinstance(msg, pyactivemq.BytesMessage))
        self.assertEqual(3, msg.bodyLength)
        self.assertEqual('\x00\x00\x00', msg.bodyBytes)

        bytesMessage = session.createBytesMessage()
        bytesMessage.bodyBytes = '\x01\x02\x03'
        self.assertEqual(3, bytesMessage.bodyLength)
        producer.send(bytesMessage)
        del producer
        msg = consumer.receive(5000)
        self.assert_(msg is not None)
        self.assert_(isinstance(msg, pyactivemq.Message))
        self.assert_(isinstance(msg, pyactivemq.BytesMessage))
        self.assertEqual(3, msg.bodyLength)
        self.assertEqual('\x01\x02\x03', msg.bodyBytes)

    def test_transaction(self):
        from pyactivemq import AcknowledgeMode
        session = self.conn.createSession(AcknowledgeMode.SESSION_TRANSACTED)
        self.assert_(session.transacted)
        topic = self.random_topic(session)
        consumer = session.createConsumer(topic)
        producer = session.createProducer(topic)
        textMessage = session.createTextMessage()
        textMessage.text = 'hello123'
        producer.send(textMessage)
        producer.send(textMessage)
        session.rollback()
        producer.send(textMessage)
        session.commit()
        msg = consumer.receive(5000)
        self.assert_(msg is not None)
        self.assertEqual('hello123', msg.text)
        # two sends were rolled back, so expect only one message
        msg = consumer.receive(500)
        self.assert_(msg is None)
        # roll session back so message is available again
        session.rollback()
        msg = consumer.receive(1000)
        self.assertEqual('hello123', msg.text)
        self.assert_(msg is not None)
        msg = consumer.receive(500)
        self.assert_(msg is None)

    def test_temporary_topic(self):
        session = self.conn.createSession()
        temptopic = session.createTemporaryTopic()
        self.assert_(len(temptopic.name) > 0)
        self.assert_(isinstance(temptopic, pyactivemq.Destination))
        self.assert_(isinstance(temptopic, pyactivemq.TemporaryTopic))
        self.assertEqual(pyactivemq.DestinationType.TEMPORARY_TOPIC,
                         temptopic.destinationType)

    def test_temporary_queue(self):
        session = self.conn.createSession()
        tempqueue = session.createTemporaryQueue()
        self.assert_(len(tempqueue.name) > 0)
        self.assert_(isinstance(tempqueue, pyactivemq.Destination))
        self.assert_(isinstance(tempqueue, pyactivemq.TemporaryQueue))
        self.assertEqual(pyactivemq.DestinationType.TEMPORARY_QUEUE,
                         tempqueue.destinationType)
