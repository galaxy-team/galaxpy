from distutils.core import setup, Extension

jupiter = Extension('jupiter',
                    include_dirs = ['jupiter/src/lib'],
                    sources = ['jupiter.cpp', 'jupiter/src/lib/libjupiter.cpp'])

jupiter.extra_compile_args = ['--std=c++11']

setup (name = 'GalaxPy',
       version = '1.0',
       description = 'A Python API to the Galaxy DCPU-16 toolchain',
       ext_modules = [jupiter])
