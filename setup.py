from distutils.core import setup, Extension

jupiter = Extension('jupiter',
                    sources = ['jupiter.cpp'])

setup (name = 'GalaxPy',
       version = '1.0',
       description = 'A Python API to the Galaxy DCPU-16 toolchain',
       ext_modules = [jupiter])
