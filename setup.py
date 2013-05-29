from distutils.core import setup, Extension

compile_args = ['-std=c++11', '-Wwrite-strings', '-Wsign-compare']

jupiter = Extension('jupiter',
                    include_dirs = ['lib/jupiter/src/lib',
                                    'lib/asteroid'],
                    sources = ['src/jupiter.cpp', 'lib/jupiter/src/lib/libjupiter.cpp'])
jupiter.extra_compile_args = compile_args

saturn = Extension('saturn',
                    include_dirs = ['lib/libsaturn/include'],
                    sources = ['src/saturn.cpp', 'src/pydevice.cpp'])
saturn.extra_compile_args = compile_args


setup (name = 'galaxpy',
       version = '0.1',
       license = 'GPL',
       description = 'A Python API to the Galaxy DCPU-16 toolchain',
       maintainer = 'David Beckley',
       maintainer_email = 'beckl.d.413@isg.edu.sa',
       url = 'http://www.github.com/galaxy-team/galaxpy',
       ext_package='galaxpy',
       ext_modules = [
           jupiter,
           saturn])
