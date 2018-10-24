#!/usr/bin/env python
import os
import sys
from numpy.distutils.core import setup, Extension

target_prefix = sys.prefix
for i in range(len(sys.argv)):
    a = sys.argv[i]
    if a == '--prefix':
        target_prefix = sys.argv[i+1]
    sp = a.split("--prefix=")
    if len(sp) == 2:
        target_prefix = sp[1]
        print('Target is:', target_prefix)
sys.path.insert(0, os.path.join(target_prefix, 'lib', 'python%i.%i' % sys.version_info[:2], 'site-packages'))
import cdat_info

libs_pth = os.path.join(sys.prefix, "lib")
setup(name="cdtime",
      description="Time utilities",
      version='3.2',
      url="http://cdat.sf.net",
      packages=['cdtime'],
      package_dir={'cdtime': 'Lib'},
      include_dirs=['Include', 'Include/py3c'] + cdat_info.cdunif_include_directories,
      ext_modules=[Extension('cdtime._cdtime',
                             ['Src/cdtimemodule.c'],
                             library_dirs=cdat_info.cdunif_library_directories,
                             libraries=cdat_info.cdunif_libraries,
                             runtime_library_dirs=[libs_pth],
                             extra_compile_args=["-L%s" % libs_pth, "-g", "-O0"],)
                   ]
      )
