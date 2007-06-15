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

import pyactivemq
from pyactivemq import ActiveMQConnectionFactory

class MessageListener(pyactivemq.MessageListener):
    def __init__(self, name, queue):
        pyactivemq.MessageListener.__init__(self)
        self.name = name
        self.queue = queue

    def onMessage(self, message):
        self.queue.put('%s got: %s' % (self.name, message.text))

f = ActiveMQConnectionFactory('tcp://localhost:61616?wireFormat=openwire')
conn = f.createConnection()

nmessages = 100
nconsumers = 3

# create a single producer
producer_session = conn.createSession()
topic = producer_session.createTopic('topic')
producer = producer_session.createProducer(topic)

# create infinite queue that is shared by consumers
import Queue
queue = Queue.Queue(0)

# Keep consumers in a list, because if we don't hold a reference to
# the consumer, it is closed
consumers = []
for i in xrange(nconsumers):
    # Create multiple consumers in separate sessions
    session = conn.createSession()
    consumer = session.createConsumer(topic)
    listener = MessageListener('consumer%d' % i, queue)
    consumer.messageListener = listener
    consumers.append(consumer)

conn.start()
textMessage = producer_session.createTextMessage()
for i in xrange(nmessages):
    textMessage.text = 'hello%d' % (i,)
    producer.send(textMessage)

qsize = nmessages * nconsumers
try:
    for i in xrange(qsize):
        message = queue.get(block=True, timeout=5)
        print message
except Queue.Empty:
    raise AssertionError, 'Expected %d messages in queue' % qsize
assert queue.empty()

conn.close()
