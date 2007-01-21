from distutils.core import setup
from distutils.extension import Extension
import os.path
import sys
if sys.platform == 'win32':
    include_dirs = [
        'C:\\Program Files\\boost\\boost_1_33_1',
        '..\\activemq-cpp\\src\\main'
        ]
    boost_lib = 'boost_python-vc71-mt-1_33_1'
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
    data_files = [
        os.path.join(boost_lib_dir, boost_lib + '.dll')
        ]
    extra_compile_args = ['/GR', '/wd4290']
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
    data_files = []
    extra_compile_args = []

import glob
files = glob.glob(os.path.join('src', 'main', '*.cpp'))

setup(name='pyactivemq',
      version='0.0.1',
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
                  depends=[]),
        ],
      data_files=data_files)
