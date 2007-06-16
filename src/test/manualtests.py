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

import time

def test_ExceptionListener():
    url = 'tcp://localhost:61613?wireFormat=stomp'
    from pyactivemq import ActiveMQConnectionFactory
    f = ActiveMQConnectionFactory(url)
    class ExceptionListener(pyactivemq.ExceptionListener):
        def onException(self, ex):
            self.ex = ex
    conn = f.createConnection()
    conn.exceptionListener = ExceptionListener()
    print 'Sleeping. Kill the broker now...'
    time.sleep(20)
    print conn.exceptionListener.ex
    del conn

if __name__ == '__main__':
    # start the broker, then run this test, then kill the broker
    test_ExceptionListener()
