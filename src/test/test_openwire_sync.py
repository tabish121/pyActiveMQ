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

from test_sync import _test_sync
import unittest

class test_openwire_sync(_test_sync, unittest.TestCase):
    def setUp(self):
        self.url = 'tcp://localhost:61616?wireFormat=openwire'
        from pyactivemq import ActiveMQConnectionFactory
        f = ActiveMQConnectionFactory(self.url)
        self.conn = f.createConnection()

    def tearDown(self):
        if hasattr(self, 'conn'):
            self.conn.close()
            del self.conn

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
        #self.assertEqual(str(msg.destination), str(queue))
        self.assertEqual(queue, msg.destination)
        msg = consumer.receive(50)
        self.assert_(msg is None)

    def _populate_MapMessage(self, mapMessage):
        mapMessage.setBoolean('bool1', True)
        mapMessage.setByte('byte1', 123)
        mapMessage.setChar('char1', 'X')
        mapMessage.setDouble('double1', 123.456)
        mapMessage.setFloat('float1', 123.456)
        mapMessage.setInt('int1', 123456)
        mapMessage.setLong('long1', 123456789)
        mapMessage.setShort('short1', 1234)
        mapMessage.setString('string1', 'hello123')

    def _check_MapMessage(self, mapMessage):
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

    def test_MapMessage(self):
        session = self.conn.createSession()
        mapMessage = session.createMapMessage()
        self.assert_(isinstance(mapMessage, pyactivemq.Message))
        self.assert_(isinstance(mapMessage, pyactivemq.MapMessage))
        self._populate_MapMessage(mapMessage)
        self._check_Message_properties(mapMessage)
        self._check_MapMessage(mapMessage)

    def test_send_MapMessage(self):
        session = self.conn.createSession()
        mapMessage = session.createMapMessage()
        self._populate_MapMessage(mapMessage)
        queue = self.random_queue(session)
        consumer = session.createConsumer(queue)
        producer = session.createProducer(queue)
        self.conn.start()
        producer.send(mapMessage)
        msg = consumer.receive(5000)
        self.assert_(msg is not None)
        self._check_MapMessage(msg)
        msg = consumer.receive(5000)
        self.assert_(msg is None)
        self.conn.close()

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
        msg = consumer1.receive(5000)
        self.assert_(msg is not None)
        # nolocal consumer shouldn't receive the message
        msg = consumer2.receive(500)
        self.assert_(msg is None)

if __name__ == '__main__':
    import sys
    unittest.main(argv=sys.argv)
