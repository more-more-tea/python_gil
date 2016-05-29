# -*- coding: utf-8 -*-
# Setup script to compile the module.

from distutils.core import setup, Extension

extension_mod = Extension( 'pub', [ 'pub.cpp' ],
        libraries=[ 'zmq', 'pthread' ] )

setup( name='pub', ext_modules=[ extension_mod ] )
