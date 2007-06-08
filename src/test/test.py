#!/usr/bin/env python

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

import os
import random
import sys
import time
import unittest

topdir = os.path.join(os.path.dirname(__file__), '..', '..')
topdir = os.path.abspath(topdir)
from distutils.util import get_platform
if get_platform() == 'win32':
    sys.path.insert(0, os.path.join(topdir, 'win_build', 'debug'))
else:
    plat_specifier = ".%s-%s" % (get_platform(), sys.version[0:3])
    build_base = os.path.join(topdir, 'build')
    build_platlib = os.path.join(build_base, 'lib' + plat_specifier)
    sys.path.insert(0, build_platlib)

import pyactivemq
print pyactivemq

class test_pyactivemq(unittest.TestCase):
    def test_dir(self):
        names = [
            'AcknowledgeMode',
            'ActiveMQConnectionFactory',
            'BytesMessage',
            'CMSException',
            'Closeable',
            'Connection',
            'ConnectionFactory',
            'DeliveryMode',
            'Destination',
            'DestinationType',
            'ExceptionListener',
            'MapMessage',
            'Message',
            'MessageConsumer',
            'MessageListener',
            'MessageProducer',
            'Queue',
            'Session',
            'Startable',
            'Stoppable',
            'TemporaryQueue',
            'TemporaryTopic',
            'TextMessage',
            'Topic'
            ]
        for name in names:
            self.assert_(name in dir(pyactivemq))

class test_AcknowledgeMode(unittest.TestCase):
    def test_values(self):
        from pyactivemq import AcknowledgeMode
        self.assertEqual(0, int(AcknowledgeMode.AUTO_ACKNOWLEDGE))
        self.assertEqual(1, int(AcknowledgeMode.DUPS_OK_ACKNOWLEDGE))
        self.assertEqual(2, int(AcknowledgeMode.CLIENT_ACKNOWLEDGE))
        self.assertEqual(3, int(AcknowledgeMode.SESSION_TRANSACTED))

class test_DeliveryMode(unittest.TestCase):
    def test_values(self):
        from pyactivemq import DeliveryMode
        self.assertEqual(0, DeliveryMode.PERSISTENT)
        self.assertEqual(1, DeliveryMode.NON_PERSISTENT)

class test_ActiveMQConnectionFactory(unittest.TestCase):
    def test_properties(self):
        from pyactivemq import ActiveMQConnectionFactory
        f1 = ActiveMQConnectionFactory()
        f2 = ActiveMQConnectionFactory('url')
        self.assertEqual('url', f2.brokerURL)
        f3 = ActiveMQConnectionFactory('url', 'user')
        self.assertEqual('url', f3.brokerURL)
        self.assertEqual('user', f3.username)
        f4 = ActiveMQConnectionFactory('url', 'user', 'password')
        self.assertEqual('url', f4.brokerURL)
        self.assertEqual('user', f4.username)
        self.assertEqual('password', f4.password)

class test_DestinationType(unittest.TestCase):
    def test_values(self):
        from pyactivemq import DestinationType
        self.assertEqual(0, DestinationType.TOPIC)
        self.assertEqual(1, DestinationType.QUEUE)
        self.assertEqual(2, DestinationType.TEMPORARY_TOPIC)
        self.assertEqual(3, DestinationType.TEMPORARY_QUEUE)

def random_topic(self, session):
    rand = random.Random()
    random_id = "%08x" % rand.randrange(0, 2**31)
    return session.createTopic("topic-%s" % random_id)

class _test_any_protocol:
    random_topic = random_topic

    def test_Connection(self):
        self.assertEqual(2, sys.getrefcount(self.conn))
        conn = self.conn
        self.assertEqual(3, sys.getrefcount(conn))
        self.assert_(isinstance(conn, pyactivemq.Closeable))
        self.assert_(isinstance(conn, pyactivemq.Connection))
        from pyactivemq import AcknowledgeMode
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
        from pyactivemq import AcknowledgeMode
        ackmode = AcknowledgeMode.AUTO_ACKNOWLEDGE
        self.assertEqual(ackmode, session.acknowledgeMode)
        try:
            # check that attribute is read-only
            session.acknowledgeMode = ackmode
            self.assert_(False, 'Expected AttributeError to be raised')
        except AttributeError:
            pass
        session.close()

    def test_Topic_and_Queue(self):
        from pyactivemq import DestinationType
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

    def xtest_durable_consumer(self):
        conn1 = self.conn
        del self.conn
        session1 = conn1.createSession()
        sub = "mysubscription1"
        # unsubscribing non-existant subscription should fail
        self.assertRaises(UserWarning, session1.unsubscribe, sub)
        topic = self.random_topic(session1)
        consumer = session1.createDurableConsumer(topic, sub, '', False)
        # unsubscribing with an active consumer should fail
        self.assertRaises(UserWarning, session1.unsubscribe, sub)
        del consumer
        # TODO this shouldn't throw
        #session1.unsubscribe(sub)
        consumer = session1.createDurableConsumer(topic, sub, '', False)

        from pyactivemq import ActiveMQConnectionFactory
        f = ActiveMQConnectionFactory(self.url)
        conn2 = f.createConnection()
        session2 = conn2.createSession()
        # Using a topic created in another session to create the
        # producer. This shouldn't cause problems.
        producer = session2.createProducer(topic)

        conn1.start()
        conn2.start()

        # TODO send a few messages using the producer
        # TODO receive messages in a transaction
        # TODO rollback transaction
        # TODO close connection
        # TODO reconnect conn1 and setup subscription
        # check that messages are still there

    def test_TextMessage(self):
        session = self.conn.createSession()
        textMessage = session.createTextMessage()
        self.assert_(isinstance(textMessage, pyactivemq.Message))
        self.assert_(isinstance(textMessage, pyactivemq.TextMessage))
        textMessage.text = "bye"
        self.assertEqual("bye", textMessage.text)
        textMessage = session.createTextMessage("hello")
        self.assertEqual("hello", textMessage.text)

        textMessage.setIntProperty('int1', 123)
        self.assertEqual(1, len(textMessage.propertyNames))
        self.assertEqual('int1', textMessage.propertyNames[0])
        self.assert_(textMessage.destination is None)
        self.assert_(textMessage.replyTo is None)

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
        self.assertEqual(0, bytesMessage.bodyLength)
        bytesMessage.bodyBytes = 'hello123'
        self.assertEqual('hello123', bytesMessage.bodyBytes)
        self.assert_(bytesMessage.replyTo is None)
        queue = session.createQueue("queue")
        bytesMessage.replyTo = queue
        self.assertEqual(queue, bytesMessage.replyTo)

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
        msg = consumer.receive(1000)

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
        msg = consumer.receive(1000)

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
        msg = consumer.receive(1000)
        self.assert_(msg is not None)
        # XXX this doesn't return anything yet
        #self.assertEqual('hello123', msg.readBytes())
        self.assertEqual('', msg.readBytes())

        msg = consumer.receive(50)
        self.assert_(msg is None)

    def xtest_MessageProducer_sends(self):
        # test all send methods of MessageProducer
        # make consumers on 2 topics so that we can check that the
        # topic specified when calling send instead of topic specified
        # when creating producer gets used

        # test that sending to a closed connection fails or something
        pass

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
        msg = consumer.receive(1000)

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
        msg = consumer.receive(1000)
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
        msg = consumer.receive(1000)
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

class test_stomp(_test_any_protocol, unittest.TestCase):
    def setUp(self):
        self.url = 'tcp://localhost:61613?wireFormat=stomp'
        from pyactivemq import ActiveMQConnectionFactory
        f = ActiveMQConnectionFactory(self.url)
        self.conn = f.createConnection()

    def tearDown(self):
        if hasattr(self, 'conn'):
            self.conn.close()
            del self.conn

    def test_temporary_topic(self):
        session = self.conn.createSession()
        # not implemented for stomp
        # XXX UserWarning says: caught unknown exception
        self.assertRaises(UserWarning, session.createTemporaryTopic)

    def test_temporary_queue(self):
        session = self.conn.createSession()
        # not implemented for stomp
        self.assertRaises(UserWarning, session.createTemporaryQueue)

    def test_MapMessage(self):
        session = self.conn.createSession()
        # not implemented for stomp
        self.assertRaises(UserWarning, session.createMapMessage)

    def test_nolocal(self):
        session = self.conn.createSession()
        textMessage = session.createTextMessage()
        topic = self.random_topic(session)
        # Consumer with empty selector and nolocal set.
        consumer = session.createConsumer(topic, "", True)
        producer = session.createProducer(topic)
        self.conn.start()
        producer.send(textMessage)
        msg = consumer.receive(500)
        # nolocal consumer shouldn't receive the message
        self.assert_(msg is None)

    def test_nolocal_multiple_consumers(self):
        session = self.conn.createSession()
        textMessage = session.createTextMessage()
        topic = self.random_topic(session)
        # consumer with empty selector
        consumer1 = session.createConsumer(topic, "")
        # Consumer with empty selector and nolocal set. Because this
        # is the second consumer created on the connection, the
        # nolocal flag has no effect.
        consumer2 = session.createConsumer(topic, "", True)
        producer = session.createProducer(topic)
        self.conn.start()
        producer.send(textMessage)
        msg = consumer1.receive(2000)
        self.assert_(msg is not None)
        # Because nolocal is ignored except for the first connection,
        # there should be a message available
        msg = consumer2.receive(2000)
        self.assert_(msg is not None)

class test_openwire(_test_any_protocol, unittest.TestCase):
    def setUp(self):
        self.url = 'tcp://localhost:61616?wireFormat=openwire'
        from pyactivemq import ActiveMQConnectionFactory
        f = ActiveMQConnectionFactory(self.url)
        self.conn = f.createConnection()

    def tearDown(self):
        if hasattr(self, 'conn'):
            self.conn.close()
            del self.conn

    def test_temporary_topic(self):
        session = self.conn.createSession()
        temptopic = session.createTemporaryTopic()
        self.assert_(len(temptopic.name) > 0)
        self.assert_(isinstance(temptopic, pyactivemq.Destination))
        self.assert_(not isinstance(temptopic, pyactivemq.Topic))
        from pyactivemq import DestinationType
        self.assertEqual(DestinationType.TEMPORARY_TOPIC,
                         temptopic.destinationType)

    def test_temporary_queue(self):
        session = self.conn.createSession()
        tempqueue = session.createTemporaryQueue()
        self.assert_(len(tempqueue.name) > 0)
        self.assert_(isinstance(tempqueue, pyactivemq.Destination))
        self.assert_(not isinstance(tempqueue, pyactivemq.Queue))
        from pyactivemq import DestinationType
        self.assertEqual(DestinationType.TEMPORARY_QUEUE,
                         tempqueue.destinationType)

    def test_MapMessage(self):
        session = self.conn.createSession()
        mapMessage = session.createMapMessage()
        self.assert_(isinstance(mapMessage, pyactivemq.Message))
        self.assert_(isinstance(mapMessage, pyactivemq.MapMessage))
        mapMessage.setBoolean('bool1', True)
        mapMessage.setByte('byte1', 123)
        mapMessage.setChar('char1', 'X')
        mapMessage.setDouble('double1', 123.456)
        mapMessage.setFloat('float1', 123.456)
        mapMessage.setInt('int1', 123456)
        mapMessage.setLong('long1', 123456789)
        mapMessage.setShort('short1', 1234)
        mapMessage.setString('string1', 'hello123')
        self.assertEqual(9, len(mapMessage.mapNames))
        for name in mapMessage.mapNames:
            self.assert_(mapMessage.itemExists(name))
        self.assertEqual(True, mapMessage.getBoolean('bool1'))
        self.assertEqual(123, mapMessage.getByte('byte1'))
        self.assertEqual('X', mapMessage.getChar('char1'))
        self.assertAlmostEqual(123.456, mapMessage.getDouble('double1'), 5)
        self.assertAlmostEqual(123.456, mapMessage.getFloat('float1'), 5)
        self.assertEqual(123456, mapMessage.getInt('int1'))
        self.assertEqual(123456789, mapMessage.getLong('long1'))
        self.assertEqual(1234, mapMessage.getShort('short1'))
        self.assertEqual('hello123', mapMessage.getString('string1'))

    def xtest_send_MapMessage(self):
        # TODO implement testing of MapMessage send
        pass

    def test_nolocal(self):
        session = self.conn.createSession()
        textMessage = session.createTextMessage()
        topic = self.random_topic(session)
        # consumer with empty selector
        consumer1 = session.createConsumer(topic, "")
        # consumer with empty selector and nolocal set
        consumer2 = session.createConsumer(topic, "", True)
        producer = session.createProducer(topic)
        self.conn.start()
        producer.send(textMessage)
        msg = consumer1.receive(2000)
        self.assert_(msg is not None)
        # nolocal consumer shouldn't receive the message
        msg = consumer2.receive(500)
        self.assert_(msg is None)

class _test_async:
    random_topic = random_topic

    def test_sessions_with_message_listeners(self):
        class MessageListener(pyactivemq.MessageListener):
            def __init__(self, queue):
                pyactivemq.MessageListener.__init__(self)
                self.queue = queue

            def onMessage(self, message):
                self.queue.put(message)

        nmessages = 100
        nconsumers = 3

        # create a single producer
        producer_session = self.conn.createSession()
        topic = self.random_topic(producer_session)
        producer = producer_session.createProducer(topic)

        # create infinite queue that is shared by consumers
        import Queue
        queue = Queue.Queue(0)

        # create multiple consumers in separate sessions
        # keep consumers in a list, because if we don't hold a
        # reference to the consumer, it is closed
        consumers = []
        for i in xrange(nconsumers):
            session = self.conn.createSession()
            consumer = session.createConsumer(topic)
            listener = MessageListener(queue)
            consumer.messageListener = listener
            consumers.append(consumer)

        self.conn.start()
        textMessage = producer_session.createTextMessage()
        for i in xrange(nmessages):
            textMessage.text = 'hello%d' % (i,)
            producer.send(textMessage)

        qsize = nmessages * nconsumers
        try:
            for i in xrange(qsize):
                message = queue.get(block=True, timeout=5)
                self.assert_(message.text.startswith('hello'))
        except Queue.Empty:
            self.assert_(False, 'Expected %d messages in queue' % qsize)
        self.assert_(queue.empty())

class test_stomp_async(_test_async, unittest.TestCase):
    def setUp(self):
        self.url = 'tcp://localhost:61613?wireFormat=stomp'
        from pyactivemq import ActiveMQConnectionFactory
        f = ActiveMQConnectionFactory(self.url)
        self.conn = f.createConnection()

    def tearDown(self):
        self.conn.close()
        del self.conn

class test_openwire_async(_test_async, unittest.TestCase):
    def setUp(self):
        self.url = 'tcp://localhost:61616?wireFormat=openwire'
        from pyactivemq import ActiveMQConnectionFactory
        f = ActiveMQConnectionFactory(self.url)
        self.conn = f.createConnection()

    def tearDown(self):
        self.conn.close()
        del self.conn

    def test_selectors(self):
        import Queue

        class MessageListener(pyactivemq.MessageListener):
            def __init__(self):
                pyactivemq.MessageListener.__init__(self)
                self.queue = Queue.Queue(0)

            def onMessage(self, message):
                self.queue.put(message)

        session = self.conn.createSession()
        topic = self.random_topic(session)
        producer = session.createProducer(topic)

        nconsumers = 7
        consumers = []
        for i in xrange(1, nconsumers + 1):
            session = self.conn.createSession()
            selector = 'int1%%%d=0' % i
            consumer = session.createConsumer(topic, selector)
            consumer.messageListener = MessageListener()
            consumers.append(consumer)

        messagecounts = [0] * nconsumers
        self.conn.start()
        textMessage = session.createTextMessage()
        nmessages = 200
        for i in xrange(nmessages):
            textMessage.setIntProperty('int1', i)
            producer.send(textMessage)
            for j in xrange(1, nconsumers + 1):
                if i % j == 0:
                    messagecounts[j - 1] += 1

        listeners = map(lambda x: x.messageListener, consumers)
        for i, (messagecount, listener) in enumerate(zip(messagecounts, listeners)):
            try:
                for j in xrange(messagecount):
                    message = listener.queue.get(block=True, timeout=5)
                    int1 = message.getIntProperty('int1')
                    self.assertEqual(0, int1 % (i + 1))
            except Queue.Empty:
                msg = 'Expected %d messages for consumer %d, got %d'
                self.assert_(False,  msg % (messagecount, i, j))
            self.assert_(listener.queue.empty())

def test_ExceptionListener():
    url = 'tcp://localhost:61613?wireFormat=stomp'
    from pyactivemq import ActiveMQConnectionFactory
    f = ActiveMQConnectionFactory(url)
    class ExceptionListener(pyactivemq.ExceptionListener):
        def onException(self, ex):
            # TODO test what happens if we keep a reference to this
            # exception object
            print ex
    conn = f.createConnection()
    conn.exceptionListener = ExceptionListener()
    time.sleep(20)
    del conn

if __name__ == '__main__':
    if True:
        unittest.main(argv=sys.argv)
    else:
        testLoader = unittest.defaultTestLoader
        module = __import__('__main__')
        test = testLoader.loadTestsFromModule(module)
        testRunner = unittest.TextTestRunner(verbosity=2)
        while True:
            result = testRunner.run(test)
