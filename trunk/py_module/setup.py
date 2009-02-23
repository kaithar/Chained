from distutils.core import setup, Extension

module1 = Extension('chained.net',
                    define_macros = [('MAJOR_VERSION', '1'),
                                     ('MINOR_VERSION', '0')],
                    include_dirs = ['/opt/chained/includes'],
                    libraries = ['chained'],
                    library_dirs = ['/opt/chained/libs'],
                    sources = ['py_module.c',
				'Connection_api.c', 'Connection_getset_funcs.c', 'pyConnection.c',
				'Port_api.c', 'pyPort.c', 'pyTimer.c'],
		    extra_compile_args = ['-ggdb'],
		    extra_link_args = ['--rpath=/opt/chained/libs'])

setup (name = 'chained',
	version = '1.0',
	description = 'Communications Library',
	ext_modules = [module1],
	packages = ['chained', 'chained.comms'],
	package_dir = {'chained': 'support'})