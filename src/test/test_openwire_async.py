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

from test_async import _test_async
import unittest
import Queue

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
        session = self.conn.createSession()
        topic = self.random_topic(session)
        producer = session.createProducer(topic)

        nconsumers = 7
        consumers = []
        for i in xrange(1, nconsumers + 1):
            session = self.conn.createSession()
            selector = 'int1%%%d=0' % i
            consumer = session.createConsumer(topic, selector)
            consumer.messageListener = self.QueueMessageListener()
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

if __name__ == '__main__':
    import sys
    unittest.main(argv=sys.argv)
