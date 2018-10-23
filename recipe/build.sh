#!/usr/bin/env bash
export CFLAGS="-Wall -g -m64 -pipe -O2  -fPIC"
export CXXLAGS="${CFLAGS}"
export CPPFLAGS="-I${PREFIX}/include"
export LDFLAGS="-L${PREFIX}/lib"

if [ $(uname) == "Linux" ];then
    export LDSHARED="$CC -shared -pthread" 
    if [ ! -f ${PREFIX}/lib/libgfortran.so ]; then
        ln -s ${PREFIX}/lib/libgfortran.so.3.0.0 ${PREFIX}/lib/libgfortran.so
    fi
    LDSHARED="$CC -shared -pthread" python setup.py install
else
    python setup.py install
fi
