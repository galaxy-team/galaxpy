from distutils.core import setup, Extension

jupiter = Extension('jupiter',
                    include_dirs = ['lib/jupiter/src/lib'],
                    sources = ['src/jupiter.cpp'])

jupiter.extra_compile_args = ['--std=c++11']

setup (name = 'galaxpy',
       version = '0.1',
       license = 'GPL',
       description = 'A Python API to the Galaxy DCPU-16 toolchain',
       ext_package='galaxpy',
       ext_modules = [jupiter])
