# -*- coding: utf-8 -*-
# Setup script to compile the module.

from distutils.core import setup, Extension

extension_mod = Extension( 'proc', [ 'proc.cpp' ],
        libraries=[ 'pthread' ] )

setup( name='proc', ext_modules=[ extension_mod ] )
