from distutils.core import setup, Extension

from os.path import join, dirname, abspath, exists
from os import chdir

current_dir = dirname(abspath(__name__))
path = join(current_dir, '..')
chdir(path)

default_lib_dir = join(current_dir, 'lib')

compile_args = [
    '-std=c++11',
    '-Wwrite-strings',
    '-Wsign-compare',
    '-fPIC',
    '-pedantic',
    '-Wall',
    '-Wextra'
]
link_args = ['-fPIC']

jupiter = Extension('jupiter',
    include_dirs=[
        'lib/jupiter/src/lib',
        'lib/asteroid'
    ],
    libraries=['jupiter', 'glog'],
    library_dirs=[default_lib_dir, 'lib/jupiter/lib', 'lib/jupiter/build/lib'],
    sources=['src/jupiter.cpp', 'lib/jupiter/src/lib/libjupiter.cpp'],
    extra_compile_args=compile_args,
    extra_link_args=link_args
)

saturn = Extension('saturn',
    include_dirs=['lib/libsaturn/include'],
    libraries=['saturn'],
    library_dirs=[default_lib_dir, 'lib/libsaturn/lib'],
    sources=['src/saturn.cpp', 'src/pydevice.cpp'],
    extra_compile_args=compile_args,
    extra_link_args=link_args
)

asteroid = Extension('asteroid',
    include_dirs=['lib/asteroid'],
    sources=['src/asteroid.cpp'],
    extra_compile_args=compile_args,
    extra_link_args=link_args,
    language='c++'
)

settings = {
    'name': 'galaxpy',
    'version': '0.1',
    'license': 'GPL',
    'description': 'A Python API to the Galaxy DCPU-16 toolchain',
    'maintainer': 'David Beckley',
    'maintainer_email': 'beckl.d.413@isg.edu.sa',
    'url': 'http://www.github.com/galaxy-team/galaxpy',
    'ext_package': 'galaxpy',
    'ext_modules': [
        jupiter,
        saturn,
        asteroid
    ]
}

setup(**settings)
