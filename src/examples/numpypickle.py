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

from pyactivemq import ActiveMQConnectionFactory
from pyactivemq import AcknowledgeMode
from pyactivemq import DeliveryMode
from numpy.testing import assert_array_equal
import numpy as N

npickles = 1000

# generate random array containing 100*100 doubles
x = N.random.randn(100, 100)

f = ActiveMQConnectionFactory('tcp://localhost:61613?wireFormat=stomp')
conn = f.createConnection()
session = conn.createSession(AcknowledgeMode.DUPS_OK_ACKNOWLEDGE)
queue = session.createQueue('arrays')
consumer = session.createConsumer(queue)
producer = session.createProducer(queue)
producer.deliveryMode = DeliveryMode.NON_PERSISTENT
conn.start()

def test():
    for i in xrange(npickles):
        m = session.createBytesMessage()
        # pickle array into BytesMessage's body
        m.bodyBytes = x.dumps()
        producer.send(m)
        m2 = consumer.receive(1000)
        assert m2 is not None
        # unpickle array from BytesMessage's body
        y = N.loads(m2.bodyBytes)
        assert_array_equal(x, y)

from timeit import Timer
t = Timer('test()', 'from __main__ import test')
delta = t.timeit(1)

conn.close()

print 'pickled %d arrays, each %d bytes, in %f seconds' % \
    (npickles, x.nbytes, delta)
