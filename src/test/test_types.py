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
import unittest

class test_pyactivemq(unittest.TestCase):
    def test_dir(self):
        names = [
            'AcknowledgeMode',
            'ActiveMQConnectionFactory',
            'BytesMessage',
            'CMSException',
            'CMSProperties',
            'CMSSecurityException',
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
            'ObjectMessage',
            'Queue',
            'QueueBrowser',
            'Session',
            'Startable',
            'Stoppable',
            'StreamMessage',
            'TemporaryQueue',
            'TemporaryTopic',
            'TextMessage',
            'Topic'
            ]
        for name in names:
            self.assert_(name in dir(pyactivemq), '%s not in dir' % name)

    def test_version(self):
        self.assertEqual('0.2.0', pyactivemq.__version__)

class test_AcknowledgeMode(unittest.TestCase):
    def test_values(self):
        from pyactivemq import AcknowledgeMode
        self.assertEqual(0, int(AcknowledgeMode.AUTO_ACKNOWLEDGE))
        self.assertEqual(1, int(AcknowledgeMode.DUPS_OK_ACKNOWLEDGE))
        self.assertEqual(2, int(AcknowledgeMode.CLIENT_ACKNOWLEDGE))
        self.assertEqual(3, int(AcknowledgeMode.SESSION_TRANSACTED))
        self.assertEqual(4, int(AcknowledgeMode.INDIVIDUAL_ACKNOWLEDGE))

class test_DeliveryMode(unittest.TestCase):
    def test_values(self):
        from pyactivemq import DeliveryMode
        self.assertEqual(0, DeliveryMode.PERSISTENT)
        self.assertEqual(1, DeliveryMode.NON_PERSISTENT)

class test_ActiveMQConnectionFactory(unittest.TestCase):
    def test_properties(self):
        from pyactivemq import ActiveMQConnectionFactory
        f1 = ActiveMQConnectionFactory()
        # default broker URL enables failover
        self.assertEqual('failover:(tcp://localhost:61616)', f1.brokerURI)
        self.assertEqual('', f1.username)
        self.assertEqual('', f1.password)
        f2 = ActiveMQConnectionFactory('url')
        self.assertEqual('url', f2.brokerURI)
        f3 = ActiveMQConnectionFactory('url', 'user')
        self.assertEqual('url', f3.brokerURI)
        self.assertEqual('user', f3.username)
        f4 = ActiveMQConnectionFactory('url', 'user', 'password')
        self.assertEqual('url', f4.brokerURI)
        self.assertEqual('user', f4.username)
        self.assertEqual('password', f4.password)
        f4.brokerURI = 'url2'
        f4.username = 'user2'
        f4.password = 'password2'
        self.assertEqual('url2', f4.brokerURI)
        self.assertEqual('user2', f4.username)
        self.assertEqual('password2', f4.password)

    def test_error(self):
        from pyactivemq import ActiveMQConnectionFactory
        f = ActiveMQConnectionFactory()
        # set broker URL with an invalid port
        f.brokerURI = 'tcp://localhost:70000'
        try:
            conn = f.createConnection()
        except Exception:
            exctype, value = sys.exc_info()[:2]
            self.assert_(exctype is pyactivemq.CMSException)

class test_DestinationType(unittest.TestCase):
    def test_values(self):
        from pyactivemq import DestinationType
        self.assertEqual(0, DestinationType.TOPIC)
        self.assertEqual(1, DestinationType.QUEUE)
        self.assertEqual(2, DestinationType.TEMPORARY_TOPIC)
        self.assertEqual(3, DestinationType.TEMPORARY_QUEUE)

if __name__ == '__main__':
    unittest.main(argv=sys.argv)
