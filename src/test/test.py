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

import os
import sys
topdir = os.path.join(os.path.dirname(__file__), '..', '..')
topdir = os.path.abspath(topdir)
from distutils.util import get_platform
if get_platform() == 'win32':
    sys.path.insert(0, os.path.join(topdir, 'win_build', 'debug'))
else:
    plat_specifier = ".%s-%s" % (get_platform(), sys.version[0:3])
    build_base = os.path.join(topdir, 'build')
    build_platlib = os.path.join(build_base, 'lib' + plat_specifier)
    sys.path.insert(0, build_platlib)

from pyactivemq import AcknowledgeMode
from pyactivemq import ActiveMQConnectionFactory
import pyactivemq
import time

class MessageListener(pyactivemq.MessageListener):
    def onMessage(self, message):
        print 'got a', message

class ExceptionListener(pyactivemq.ExceptionListener):
    def onException(self, exc):
        print 'got a', exc, 'exception'

assert 0 == int(AcknowledgeMode.AUTO_ACKNOWLEDGE)
assert 1 == int(AcknowledgeMode.DUPS_OK_ACKNOWLEDGE)
assert 2 == int(AcknowledgeMode.CLIENT_ACKNOWLEDGE)
assert 3 == int(AcknowledgeMode.SESSION_TRANSACTED)

f1 = ActiveMQConnectionFactory()
f2 = ActiveMQConnectionFactory('url')
assert f2.brokerURL == 'url'
f3 = ActiveMQConnectionFactory('url', 'user')
assert f3.brokerURL == 'url'
assert f3.username == 'user'
f4 = ActiveMQConnectionFactory('url', 'user', 'pass')
assert f4.brokerURL == 'url'
assert f4.username == 'user'
assert f4.password == 'pass'
f5 = ActiveMQConnectionFactory('url', 'user', 'pass', 'clientid')
assert f5.brokerURL == 'url'
assert f5.username == 'user'
assert f5.password == 'pass'
assert f5.clientId == 'clientid'

f = ActiveMQConnectionFactory('tcp://localhost:61613')
try:
    conn = f.createConnection()
except UserWarning:
    print >>sys.stderr, 'Connection failed. Is the broker running?'
    sys.exit(1)
assert conn.exceptionListener is None
# XXX need to sort out a few more issues with exception listeners
conn.exceptionListener = ExceptionListener()
session = conn.createSession()
assert not session.transacted

topic = session.createTopic("topic")
topic2 = session.createTopic("topic2")
queue = session.createQueue("queue")
queue2 = session.createQueue("queue2")

assert topic == topic
assert topic != topic2
assert topic.destinationType == topic2.destinationType
assert topic != queue
assert queue == queue
assert queue != queue2
assert queue.destinationType == queue2.destinationType
assert queue != topic

from pyactivemq import DestinationType
assert DestinationType.TOPIC == 0
assert DestinationType.QUEUE == 1
assert DestinationType.TEMPORARY_TOPIC == 2
assert DestinationType.TEMPORARY_QUEUE == 3

try:
    temptopic = session.createTemporaryTopic()
    assert False
except UserWarning:
    # not implemented for stomp
    # XXX UserWarning says: caught unknown exception
    pass

try:
    tempqueue = session.createTemporaryQueue()
    assert False
except UserWarning:
    pass

textMessage = session.createTextMessage()
assert isinstance(textMessage, pyactivemq.Message)
assert isinstance(textMessage, pyactivemq.TextMessage)
textMessage.text = "bye"
assert textMessage.text == "bye"
textMessage = session.createTextMessage("hello")
assert textMessage.text == "hello"

textMessage.setIntProperty('int1', 123)
assert 1 == len(textMessage.propertyNames)
assert 'int1' == textMessage.propertyNames[0]
assert textMessage.destination is None
assert textMessage.replyTo is None
textMessage.replyTo = queue

bytesMessage = session.createBytesMessage()
assert isinstance(bytesMessage, pyactivemq.Message)
assert isinstance(bytesMessage, pyactivemq.BytesMessage)
assert bytesMessage.bodyLength == 0
bytesMessage.bodyBytes = 'hello123'
assert 'hello123' == bytesMessage.bodyBytes
bytesMessage.replyTo = topic

try:
    mapMessage = session.createMapMessage()
    assert False

    assert isinstance(mapMessage, pyactivemq.Message)
    assert isinstance(mapMessage, pyactivemq.MapMessage)
    mapMessage.setInt('int1', 123)
    assert 123 == mapMessage.getInt('int1')
    assert len(mapMessage.mapNames) == 1
    assert 'int1' in mapMessage.mapNames
except UserWarning:
    # not implemented for stomp
    pass

consumer = session.createConsumer(topic)
assert consumer.messageListener is None
consumer2 = session.createConsumer(topic)
# XXX doesn't work yet
#consumer2.messageListener = MessageListener()
producer = session.createProducer(topic)
conn.start()

producer.send(textMessage)
msg = consumer.receive(1000)
assert msg is not None
print msg
assert str(msg.destination) == str(topic)
assert msg.destination == topic
assert str(msg.replyTo) == str(queue)
assert msg.replyTo == queue
assert isinstance(msg, pyactivemq.Message)
assert isinstance(msg, pyactivemq.TextMessage)
msg = consumer.receive(50)
assert msg is None

producer.send(bytesMessage)
msg = consumer.receive(1000)
assert msg is not None
print msg
assert 'hello123' == msg.bodyBytes
assert msg.destination == topic
assert str(msg.destination) == str(topic)
assert str(msg.replyTo) == str(topic)
assert msg.replyTo == topic
assert isinstance(msg, pyactivemq.Message)
assert isinstance(msg, pyactivemq.BytesMessage)

bytesMessage = session.createBytesMessage()
bytesMessage.writeBytes('hello123')
producer.send(bytesMessage)
msg = consumer.receive(1000)
assert msg is not None
# XXX this crashes
#assert 'hello123' == msg.readBytes()

# XXX Sleep, let exception listener fire and then do keyboard
# interrupt.  Leads to a crash while deleting Connection object, which
# looks similar to what is reported here:
# http://issues.apache.org/activemq/browse/AMQCPP-46
#time.sleep(100000)

session.close()
conn.close()
