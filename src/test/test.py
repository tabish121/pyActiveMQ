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
print get_platform()
if sys.platform == 'win32':
    sys.path.insert(0, os.path.join(topdir, 'win_build', 'debug'))
else:
    plat_specifier = ".%s-%s" % (get_platform(), sys.version[0:3])
    build_base = os.path.join(topdir, 'build')
    build_platlib = os.path.join(build_base, 'lib' + plat_specifier)
    sys.path.insert(0, build_platlib)

import time

import pyactivemq

from pyactivemq import AcknowledgeMode
print int(AcknowledgeMode.AUTO_ACKNOWLEDGE)
print int(AcknowledgeMode.DUPS_OK_ACKNOWLEDGE)
print int(AcknowledgeMode.CLIENT_ACKNOWLEDGE)
print int(AcknowledgeMode.SESSION_TRANSACTED)

from pyactivemq import ActiveMQConnectionFactory
print ActiveMQConnectionFactory
f1 = ActiveMQConnectionFactory()
print f1
f2 = ActiveMQConnectionFactory('url')
print f2
f3 = ActiveMQConnectionFactory('url', 'user')
#print f3.getUsername()
f4 = ActiveMQConnectionFactory('url', 'user', 'pass')
print f4
f5 = ActiveMQConnectionFactory('url', 'user', 'pass', 'clientid')
print f5

f = ActiveMQConnectionFactory('tcp://localhost:61613')
conn = f.createConnection()
print conn
print conn.clientId
session = conn.createSession()
print session
print session.transacted

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

#print session.createConsumer
textMessage = session.createTextMessage()
print textMessage
textMessage.text = "bye"
assert textMessage.text == "bye"
textMessage = session.createTextMessage("hello")
assert textMessage.text == "hello"

textMessage.setIntProperty('int1', 123)
print len(textMessage.propertyNames)
# XXX this crashes
#print textMessage.propertyNames[0]

bytesMessage = session.createBytesMessage()
print bytesMessage
assert bytesMessage.bodyLength == 0

class MessageListener(pyactivemq.MessageListener):
    def onMessage(message):
        print 'got a', message

class ExceptionListener(pyactivemq.ExceptionListener):
    def onException(exc):
        print 'got a', exc, 'exception'

MessageListener()
ExceptionListener()

consumer = session.createConsumer(topic)
# XXX doesn't work yet -- need to bind message listener methods
#consumer2 = session.createConsumer(topic)
#consumer2.messageListener = MessageListener()
producer = session.createProducer(topic)
conn.start()
producer.send(textMessage)
msg = consumer.receive()
print 'received a', msg

session.close()
conn.close()
