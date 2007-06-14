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

class test_stomp_async(_test_async, unittest.TestCase):
    def setUp(self):
        self.url = 'tcp://localhost:61613?wireFormat=stomp'
        from pyactivemq import ActiveMQConnectionFactory
        f = ActiveMQConnectionFactory(self.url)
        self.conn = f.createConnection()

    def tearDown(self):
        self.conn.close()
        del self.conn

if __name__ == '__main__':
    import sys
    unittest.main(argv=sys.argv)
