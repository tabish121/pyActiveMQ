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

import os.path
import random
import sys

__all__ = [
    'set_local_path',
    'restore_path',
    'random_topic',
    'random_queue'
    ]

def set_local_path():
    topdir = os.path.join(os.path.dirname(__file__), '..', '..')
    topdir = os.path.abspath(topdir)
    from distutils.util import get_platform
    if True and get_platform().startswith('win'):
        if get_platform() == 'win-amd64':
            builddir = 'Release.x64'
        else:
            builddir = 'Release.Win32'
        sys.path.insert(0, os.path.join(topdir, 'win_build', builddir))
    else:
        plat_specifier = ".%s-%s" % (get_platform(), sys.version[0:3])
        build_base = os.path.join(topdir, 'build')
        build_platlib = os.path.join(build_base, 'lib' + plat_specifier)
        sys.path.insert(0, build_platlib)

def restore_path():
    del sys.path[0]

def random_topic(self, session):
    rand = random.Random()
    random_id = "%08x" % rand.randrange(0, 2**31)
    return session.createTopic("topic-%s" % random_id)

def random_queue(self, session):
    rand = random.Random()
    random_id = "%08x" % rand.randrange(0, 2**31)
    return session.createQueue("queue-%s" % random_id)
