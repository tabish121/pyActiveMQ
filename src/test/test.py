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
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..', 'win_build', 'debug'))

from pyactivemq import ActiveMQConnectionFactory
print ActiveMQConnectionFactory
f1 = ActiveMQConnectionFactory()
print f1
f2 = ActiveMQConnectionFactory('url')
print f2
f3 = ActiveMQConnectionFactory('url', 'user')
print f3.getUsername()
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

print session.createTopic
print session.createQueue
#textMessage = session.createTextMessage()
