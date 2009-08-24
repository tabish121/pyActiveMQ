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
from pyactivemq import CMSException
restore_path()

from test_sync import _test_sync
import unittest

class test_stomp_sync(_test_sync, unittest.TestCase):
    def setUp(self):
        self.url = 'tcp://localhost:61613?wireFormat=stomp'
        from pyactivemq import ActiveMQConnectionFactory
        f = ActiveMQConnectionFactory(self.url)
        self.conn = f.createConnection()

    def tearDown(self):
        if hasattr(self, 'conn'):
            self.conn.close()
            del self.conn
    
    # with AMQCPP-3, it seems you only get the exception when trying
    # to send the unsupported message type
    def xtest_MapMessage(self):
        session = self.conn.createSession()
        # not implemented for stomp
        self.assertRaises(CMSException, session.createMapMessage)
        try:
            session.createMapMessage()
            self.assert_(False)
        except CMSException, e:
            self.assert_(e.message.find('No Stomp Support') >= 0)

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
        msg = consumer1.receive(5000)
        self.assert_(msg is not None)
        # Because nolocal is ignored except for the first connection,
        # there should be a message available
        msg = consumer2.receive(5000)
        self.assert_(msg is not None)

if __name__ == '__main__':
    import sys
    unittest.main(argv=sys.argv)
