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

#
# This example corresponds to DurableSubscriberExample.java from the
# Java EE 5 Tutorial, Chapter 31, Creating Robust JMS Applications.
#

import pyactivemq
from pyactivemq import AcknowledgeMode
from threading import Event

class DurableSubscriber:
    class TextListener(pyactivemq.MessageListener):
        def __init__(self):
            pyactivemq.MessageListener.__init__(self)
            self.monitor = Event()

        def onMessage(self, message):
            if isinstance(message, pyactivemq.TextMessage):
                print 'SUBSCRIBER: Reading Message: ' + message.text
            else:
                self.monitor.set()

    def __init__(self, connectionFactory, topicName):
        connection = connectionFactory.createConnection()
        # XXX should throw without this
        #connection.clientID = 'client1234'
        session = connection.createSession(AcknowledgeMode.AUTO_ACKNOWLEDGE)
        topic = session.createTopic(topicName)
        self.connection = connection
        self.session = session
        self.topic = topic

    def startSubscriber(self):
        print 'Starting subscriber'
        self.connection.stop()
        subscriber = self.session.createDurableConsumer(self.topic, "MakeItLast", '', False)
        listener = self.TextListener()
        subscriber.messageListener = listener
        self.subscriber = subscriber
        self.connection.start()

    def closeSubscriber(self):
        subscriber = self.subscriber
        listener = subscriber.messageListener
        listener.monitor.wait()
        print 'Closing subscriber'
        subscriber.close()

    def finish(self):
        self.session.unsubscribe("MakeItLast")
        self.connection.close()

class MultiplePublisher:
    def __init__(self, connectionFactory, topicName):
        connection = connectionFactory.createConnection()
        session = connection.createSession(AcknowledgeMode.AUTO_ACKNOWLEDGE)
        topic = session.createTopic(topicName)
        producer = session.createProducer(topic)
        self.connection = connection
        self.session = session
        self.producer = producer
        self.startindex = 0

    def publishMessages(self):
        NUMMSGS = 3
        MSG_TEXT = 'Here is a message'
        message = self.session.createTextMessage()
        for i in xrange(self.startindex, self.startindex + NUMMSGS):
            message.text = MSG_TEXT + ' %d' % (i+1)
            print 'PUBLISHER: Publishing message: ' + message.text
            self.producer.send(message)
        self.startindex = self.startindex + NUMMSGS
        self.producer.send(self.session.createMessage())

    def finish(self):
        self.connection.close()

def main():
    url = 'tcp://localhost:61616'
    topicName = 'topic-1234'
    f = pyactivemq.ActiveMQConnectionFactory(url)
    durableSubscriber = DurableSubscriber(f, topicName)
    multiplePublisher = MultiplePublisher(f, topicName)
    durableSubscriber.startSubscriber()
    multiplePublisher.publishMessages()
    durableSubscriber.closeSubscriber()
    multiplePublisher.publishMessages()
    durableSubscriber.startSubscriber()
    durableSubscriber.closeSubscriber()
    multiplePublisher.finish()
    durableSubscriber.finish()

if __name__ == '__main__':
    main()
