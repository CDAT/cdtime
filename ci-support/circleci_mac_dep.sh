#!/usr/bin/env bash
ls
pwd
export PATH=${HOME}/miniconda/bin:${PATH}
conda create -n py3 -c uvcdat/label/nightly -c conda-forge -c uvcdat cdat_info libcdms libdrs_f numpy  flake8 nose "python>3"
conda create -n py2 -c uvcdat/label/nightly -c conda-forge -c uvcdat cdat_info libcdms libdrs_f numpy  flake8 nose
export UVCDAT_ANONYMOUS_LOG=False
source activate py3
python setup.py install
source activate py2
rm -rf build
python setup.py install
