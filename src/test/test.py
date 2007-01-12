import os
import sys
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..', 'win_build', 'debug'))

import pyactivemq
w = pyactivemq.World()
w.set("foo")
print w.greet()


from pyactivemq import ActiveMQConnectionFactory
print ActiveMQConnectionFactory
f1 = ActiveMQConnectionFactory()
print f1
f2 = ActiveMQConnectionFactory('url')
print f2
f3 = ActiveMQConnectionFactory('url', 'user')
print f3
f4 = ActiveMQConnectionFactory('url', 'user', 'pass')
print f4
f5 = ActiveMQConnectionFactory('url', 'user', 'pass', 'clientid')
print f5
