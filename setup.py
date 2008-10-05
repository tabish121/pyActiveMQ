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

from distutils.core import setup
from distutils.extension import Extension
from distutils.util import get_platform
import os.path
import sys

if get_platform().startswith('win'):
    include_dirs = [
        '..\\activemq-cpp\\src\\main'
        ]
    if get_platform() == 'win-amd64':
        include_dirs += [
            'C:\\Program Files\\boost\\boost_1_36_0',
            ]
    else:
        include_dirs += [
            'C:\\Program Files (x86)\\boost\\boost_1_36_0',
            ]
    libraries = [
        'libactivemq-cpp',
        'apr-1',
        'aprutil-1',
        'apriconv-1',
        'uuid',
        'ws2_32',
        'rpcrt4',
        'mswsock',
        'advapi32',
        'shell32'
        ]
    if get_platform() == 'win-amd64':
        library_dirs = [
            'win_build\\Release.x64',
            '..\\apr\\x64\\LibR',
            '..\\apr-util\\x64\\LibR',
            '..\\apr-iconv\\x64\\LibR',
            'C:\\Program Files\\boost\\boost_1_36_0\\lib'
            ]
    else:
        library_dirs = [
            'win_build\\Release.Win32',
            '..\\apr\\LibR',
            '..\\apr-util\\LibR',
            '..\\apr-iconv\\LibR',
            'C:\\Program Files (x86)\\boost\\boost_1_36_0\\lib'
            ]
    extra_compile_args = ['/EHsc', '/GR', '/wd4290']
    extra_link_args = ['/LTCG']
    define_macros = [
        ('BOOST_PYTHON_STATIC_LIB', 1),
        ('BOOST_PYTHON_NO_PY_SIGNATURES', 1),
        ('PYACTIVEMQ_ENABLE_DOCSTRINGS', 0)
        ]
else:
    include_dirs = [
        '/opt/activemq-cpp-2.2.1/include/activemq-cpp-2.2.1'
        ]
    libraries = [
        'activemq-cpp',
        'uuid',
        'boost_python'
        ]
    library_dirs = [
        '/opt/activemq-cpp-2.2.1/lib'
        ]
    extra_compile_args = []
    extra_link_args = [
        '-Wl,-rpath,/opt/activemq-cpp-2.2.1/lib'
        ]
    define_macros = [
        ('BOOST_PYTHON_NO_PY_SIGNATURES', 1),
        ('PYACTIVEMQ_ENABLE_DOCSTRINGS', 0)
        ]

import glob
files = glob.glob(os.path.join('src', 'main', '*.cpp'))
ext = Extension('pyactivemq',
                files,
                library_dirs=library_dirs,
                libraries=libraries,
                include_dirs=include_dirs,
                extra_compile_args=extra_compile_args,
                extra_link_args=extra_link_args,
                depends=[],
                define_macros=define_macros)
setup(name='pyactivemq',
      version='0.1.0',
      author='Albert Strasheim',
      author_email='fullung@gmail.com',
      url='http://code.google.com/p/pyactivemq/',
      ext_modules=[ext])
