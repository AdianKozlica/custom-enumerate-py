from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as _build_ext

class build_ext(_build_ext):
    def finalize_options(self):
        super().finalize_options()
        self.build_lib = 'build'

module = Extension('enumerate_module',
                   sources=['src/enumerate.c'])

setup(name='enumerate_module',
      version='1.0',
      description='This is a custom range module just for experimenting',
      cmdclass={'build_ext': build_ext},
      ext_modules=[module])