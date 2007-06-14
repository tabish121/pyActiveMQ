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

import unittest

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

if __name__ == '__main__':
    import sys
    unittest.main(argv=sys.argv)
