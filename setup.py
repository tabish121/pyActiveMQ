from distutils.core import setup
from distutils.extension import Extension
import os.path
import sys
if sys.platform == 'win32':
    include_dirs=[
        'C:/Program Files/boost/boost_1_33_1',
        '../activemq-cpp/src/main'
        ]
    libraries=[
        'activemq-cpp',
        'uuid',
        'libboost_python-vc71-mt-1_33_1',
        'ws2_32',
        'rpcrt4'
        ]
    library_dirs=[
        'win_build/release',
        'C:/Program Files/boost/boost_1_33_1/lib'
        ]
else:
    include_dirs = [
        '/usr/include',
        '../activemq-cpp/src/main'
        ]
    libraries=[
        'activemq-cpp',
        'uuid',
        'boost_python'
        ]
    library_dirs=[
        '/usr/lib',
        #'/path/to/amqcpp/lib'
        ]

import glob
files = glob.glob(os.path.join('src', 'main', '*.cpp'))

setup(name='pyactivemq',
      ext_modules=[
        Extension('pyactivemq', files,
                  library_dirs=library_dirs,
                  libraries=libraries,
                  include_dirs=include_dirs,
                  depends=[]),
        ])
