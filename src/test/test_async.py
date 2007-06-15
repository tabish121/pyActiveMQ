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

import Queue

class _test_async:
    random_topic = random_topic

    class QueueMessageListener(pyactivemq.MessageListener):
        def __init__(self, queue=None):
            # If the message listener class implements a
            # constructor but doesn't call the super constructor,
            # a Boost.Python.ArgumentError is raised.
            pyactivemq.MessageListener.__init__(self)
            if queue is not None:
                self.queue = queue
            else:
                self.queue = Queue.Queue(0)

        def onMessage(self, message):
            self.queue.put(message)

    def test_sessions_with_message_listeners(self):
        nmessages = 100
        nconsumers = 3

        # create a single producer
        producer_session = self.conn.createSession()
        topic = self.random_topic(producer_session)
        producer = producer_session.createProducer(topic)

        # create infinite queue that is shared by consumers
        queue = Queue.Queue(0)

        # create multiple consumers in separate sessions
        # keep consumers in a list, because if we don't hold a
        # reference to the consumer, it is closed
        consumers = []
        for i in xrange(nconsumers):
            session = self.conn.createSession()
            consumer = session.createConsumer(topic)
            listener = self.QueueMessageListener(queue)
            consumer.messageListener = listener
            consumers.append(consumer)

        self.conn.start()
        textMessage = producer_session.createTextMessage()
        for i in xrange(nmessages):
            textMessage.text = 'hello%d' % (i,)
            producer.send(textMessage)

        qsize = nmessages * nconsumers
        try:
            for i in xrange(qsize):
                message = queue.get(block=True, timeout=5)
                self.assert_(message.text.startswith('hello'))
        except Queue.Empty:
            self.assert_(False, 'Expected %d messages in queue' % qsize)
        self.assert_(queue.empty())
