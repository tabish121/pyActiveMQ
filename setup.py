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
import os.path
import sys
if sys.platform == 'win32':
    include_dirs = [
        'C:\\Program Files\\boost\\boost_1_33_1',
        '..\\activemq-cpp\\src\\main'
        ]
    boost_lib = 'libboost_python-vc71-mt-1_33_1'
    libraries = [
        'activemq-cpp',
        'uuid',
        boost_lib,
        'ws2_32',
        'rpcrt4'
        ]
    boost_lib_dir = 'C:\\Program Files\\boost\\boost_1_33_1\\lib'
    library_dirs = [
        'win_build\\release',
        boost_lib_dir
        ]
    extra_compile_args = ['/GR', '/wd4290']
    define_macros = [('BOOST_PYTHON_STATIC_LIB', 1)]
else:
    include_dirs = [
        '../activemq-cpp/src/main'
        ]
    libraries = [
        'activemq-cpp',
        'uuid',
        'boost_python'
        ]
    library_dirs = [
        #'/path/to/amqcpp/lib'
        ]
    extra_compile_args = []
    define_macros = []

import glob
files = glob.glob(os.path.join('src', 'main', '*.cpp'))

setup(name='pyactivemq',
      version='0.0.2',
      author='Albert Strasheim',
      author_email='fullung@gmail.com',
      url='http://code.google.com/p/pyactivemq/',
      ext_modules=[
        Extension('pyactivemq',
                  files,
                  library_dirs=library_dirs,
                  libraries=libraries,
                  include_dirs=include_dirs,
                  extra_compile_args=extra_compile_args,
                  depends=[],
                  define_macros=define_macros),
        ]
      )
