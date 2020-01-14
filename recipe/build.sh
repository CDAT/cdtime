
if [ `uname` == Linux ]; then
    export LDSHARED="$CC -shared -pthread" 
fi

${PYTHON} setup.py install
