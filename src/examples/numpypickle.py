#!/usr/bin/env python

from pyactivemq import ActiveMQConnectionFactory
from numpy.testing import assert_array_equal
import numpy as N

npickles = 50

# generate random array containing 100*100 doubles
x = N.random.randn(100, 100)

f = ActiveMQConnectionFactory('tcp://localhost:61613?wireFormat=stomp')
conn = f.createConnection()
session = conn.createSession()
queue = session.createQueue('arrays')
consumer = session.createConsumer(queue)
producer = session.createProducer(queue)
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
