#!/usr/bin/env python
from distutils.core import setup
import subprocess
import os
import tempfile
import shutil
import sys

src_dir = os.path.dirname(os.path.realpath(__file__))

os.chdir(src_dir)
GIT_DESCRIBE = subprocess.Popen(["git","describe","--tag"],stdout=subprocess.PIPE).stdout.read().strip()

tmp_dir = os.path.join(tempfile.gettempdir(),"cdtime_info_Lib")
shutil.rmtree(tmp_dir,ignore_errors=True)
os.makedirs(tmp_dir)
os.chdir(tmp_dir)
lib_dir = os.path.join(tmp_dir,"cdtime_info_Lib")
shutil.copytree(os.path.join(src_dir,"Libcdtime"),lib_dir)



f=open(os.path.join(src_dir,"cdtime_info.py.in"))
cdtime_info = f.read().replace("GIT_DESCRIBE",GIT_DESCRIBE)
f.close()
f=open(os.path.join(lib_dir,"cdtime_info.py"),"w")
f.write(cdtime_info)
f.close()

setup (name = "cdtime_info",
       packages = ['cdtime_info'],
       package_dir = {'cdtime_info': lib_dir},
      )
#shutil.copy(os.path.join(lib_dir,"..","build","lib","cdtime_info")+"/cdtime_info.py",os.path.join(src_dir,"Lib"))
if os.path.isdir(os.path.join(src_dir,"Libcdtimeinfo")):
    shutil.rmtree(os.path.join(src_dir,"Libcdtimeinfo"),ignore_errors=True)

shutil.copytree(os.path.join(lib_dir,"..","build","lib","cdtime_info"),os.path.join(src_dir,"Libcdtimeinfo"))
shutil.rmtree(lib_dir,ignore_errors=True)

os.chdir(src_dir)
from distutils.core import setup, Extension
import os, sys
target_prefix = sys.prefix
for i in range(len(sys.argv)):
    a = sys.argv[i]
    if a=='--prefix':
        target_prefix=sys.argv[i+1]
    sp = a.split("--prefix=")
    if len(sp)==2:
        target_prefix=sp[1]
        print 'Target is:',target_prefix
sys.path.insert(0,os.path.join(target_prefix,'lib','python%i.%i' % sys.version_info[:2],'site-packages')) 
import cdtime_info
        
setup (name = "cdtime",
       description = "Time utilities",
       version='3.2',
       url = "http://cdat.sf.net",
       include_dirs = ['Include'] + cdtime_info.cdunif_include_directories,
       ext_modules = [Extension('cdtime', 
                       ['Src/cdtimemodule.c'],
                       library_dirs = cdtime_info.cdunif_library_directories,
                       libraries = cdtime_info.cdunif_libraries)
       ]
)
shutil.rmtree(os.path.join(src_dir,"Libcdtimeinfo"),ignore_errors=True)
