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
    def onMessage(message):
        print 'got a', message

class ExceptionListener(pyactivemq.ExceptionListener):
    def onException(exc):
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
conn = f.createConnection()
session = conn.createSession()
assert not session.transacted

topic = session.createTopic("topic")
queue = session.createQueue("queue")

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
textMessage.text = "bye"
assert textMessage.text == "bye"
textMessage = session.createTextMessage("hello")
assert textMessage.text == "hello"

textMessage.setIntProperty('int1', 123)
assert 1 == len(textMessage.propertyNames)
# XXX this crashes
#print textMessage.propertyNames[0]

bytesMessage = session.createBytesMessage()
assert bytesMessage.bodyLength == 0

MessageListener()
ExceptionListener()

consumer = session.createConsumer(topic)
# XXX doesn't work yet
#consumer2 = session.createConsumer(topic)
#consumer2.messageListener = MessageListener()
producer = session.createProducer(topic)
conn.start()
producer.send(textMessage)
msg = consumer.receive()
print 'received a', msg

session.close()
conn.close()
