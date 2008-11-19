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

import unittest

from test_openwire_async import *
from test_openwire_sync import *
from test_stomp_async import *
from test_stomp_sync import *
from test_types import *

if __name__ == '__main__':
    testLoader = unittest.defaultTestLoader
    module = __import__('__main__')
    test = testLoader.loadTestsFromModule(module)
    testRunner = unittest.TextTestRunner(verbosity=2)
    for i in xrange(1000):
        result = testRunner.run(test)
