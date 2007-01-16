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
        'boost_python-vc71-mt-gd-1_33_1'
        ]
    library_dirs=[
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
