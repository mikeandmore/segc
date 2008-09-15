#!/usr/bin/env python

from distutils.core import setup, Extension


csegc_src = [
    'mempool.c',
    'avltree.c',
    'hash.c',
    'word.c',
    'dict.c',
    'chunk.c',
    'filter.c',
    'algor.c',
    'pymodule.c']

csegc_cflags = ['-O3','-march=prescott', '-ffast-math', '-mfpmath=sse']

csegc = Extension('csegc',
                  ['segc/' + fn for fn in csegc_src],
                  extra_compile_args = csegc_cflags)

setup(name = 'segc',
      version = '1.0',
      description = 'a high performance mmseg implement',
      author = 'Mike Qin',
      author_email = 'mikeandmore@gmail.com',
      url = '',
      ext_modules = [csegc],
      packages = ['segc'])

