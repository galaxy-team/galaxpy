from distutils.core import setup, Extension

jupiter = Extension('jupiter',
                    include_dirs = ['lib/jupiter/src/lib',
                                    'lib/asteroid'],
                    sources = ['src/jupiter.cpp', 'lib/jupiter/src/lib/libjupiter.cpp'])

jupiter.extra_compile_args = ['--std=c++11']

setup (name = 'galaxpy',
       version = '0.1',
       license = 'GPL',
       description = 'A Python API to the Galaxy DCPU-16 toolchain',
       maintainer = 'David Beckley',
       maintainer_email = 'beckl.d.413@isg.edu.sa',
       url = 'http://www.github.com/galaxy-team/galaxpy',
       ext_package='galaxpy',
       ext_modules = [jupiter])
