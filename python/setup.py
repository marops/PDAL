#!/usr/bin/env python

# Stolen from Shapely's setup.py
# Two environment variables influence this script.
#
# PDAL_LIBRARY_PATH: a path to a PDAL C++ shared library.
#
# GEOS_CONFIG: the path to a geos-config program that points to GEOS version,
# headers, and libraries.
#
# NB: within this setup scripts, software versions are evaluated according
# to https://www.python.org/dev/peps/pep-0440/.

import errno
import glob
import logging
import os
import platform
import re
import shutil
import subprocess
import sys
import numpy
from Cython.Build import cythonize

USE_CYTHON = True
try:
    from Cython.Build import cythonize
except ImportError:
    USE_CYTHON = False

ext = '.pyx' if USE_CYTHON else '.cpp'

from setuptools import setup
from packaging.version import Version


logging.basicConfig()
log = logging.getLogger(__file__)

# python -W all setup.py ...
if 'all' in sys.warnoptions:
    log.level = logging.DEBUG

# Get the version from the shapely module
module_version = None
with open('pdal/__init__.py', 'r') as fp:
    for line in fp:
        if line.startswith("__version__"):

            module_version = Version(
                line.split("=")[1].strip().strip("\"'"))
            break

if not module_version:
    raise ValueError("Could not determine PDAL's version")

# log.debug('GEOS shared library: %s %s', geos_version_string, geos_version)

# Handle UTF-8 encoding of certain text files.
open_kwds = {}
if sys.version_info >= (3,):
    open_kwds['encoding'] = 'utf-8'

with open('VERSION.txt', 'w', **open_kwds) as fp:
    fp.write(str(module_version))

with open('README.rst', 'r', **open_kwds) as fp:
    readme = fp.read()

with open('../AUTHORS.txt', 'r', **open_kwds) as fp:
    credits = fp.read()

with open('CHANGES.txt', 'r', **open_kwds) as fp:
    changes = fp.read()

long_description = readme + '\n\n' + credits + '\n\n' + changes

include_dirs = []
library_dirs = []
libraries = []
extra_link_args = []

from setuptools.extension import Extension as DistutilsExtension


include_dirs.append(numpy.get_include())
include_dirs.append('../include')
include_dirs.append('../plugins/python/plang/')
include_dirs.append('/usr/include/libxml2/')
library_dirs.append('../lib')
libraries.append('pdalcpp')
libraries.append('pdal_plang')

sources=['pdal/libpdalpython'+ext,"pdal/Pipeline.cpp",  ]

extensions = [DistutilsExtension("*",
                                   sources,
                                   include_dirs=include_dirs,
                                   library_dirs=library_dirs,
                                   extra_compile_args=['-std=c++11','-g','-O0'],
                                   libraries=libraries,
                                   extra_link_args=extra_link_args,)]
if USE_CYTHON:
    from Cython.Build import cythonize
    extensions= cythonize(extensions, language="c++")

setup_args = dict(
    name                = 'PDAL',
    version             = str(module_version),
    requires            = ['Python (>=2.7)', ],
    description         = 'Point cloud data processing',
    license             = 'BSD',
    keywords            = 'point cloud geospatial',
    author              = 'Howard Butler',
    author_email        = 'howard@hobu.co',
    maintainer          = 'Howard Butler',
    maintainer_email    = 'howard@hobu.co',
    url                 = 'http://pdal.io',
    long_description    = long_description,
    test_suite          = 'test',
    packages            = [
        'pdal',
    ],
    classifiers         = [
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: BSD License',
        'Operating System :: OS Independent',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Topic :: Scientific/Engineering :: GIS',
    ],
#     data_files         = [('shapely', ['shapely/_geos.pxi'])],
    cmdclass           = {},
)
setup(ext_modules=extensions, **setup_args)

