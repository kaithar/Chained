#! /usr/bin/python

from distutils.core import setup, Extension

module1 = Extension('chained.primative.net',
                    define_macros = [('MAJOR_VERSION', '1'),
                                     ('MINOR_VERSION', '0')],
                    libraries = ['chained'],
                    sources = ['py_module.c',
				'Connection_api.c', 'Connection_getset_funcs.c', 'pyConnection.c',
				'Port_api.c', 'pyPort.c', 'pyTimer.c'],
		    extra_compile_args = ['-ggdb'])

setup (name = 'chained',
	version = '1.0',
	description = 'Communications Library',
	ext_modules = [module1],
	packages = ['chained', 'chained.comms', 'chained.net', 'chained.primative'],
	package_dir = {'chained': 'support'})
