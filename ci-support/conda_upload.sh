#!/usr/bin/env bash
PKG_NAME=cdtime
USER=uvcdat
export PATH="$HOME/miniconda/bin:$PATH"
echo "Trying to upload conda"
if [ $(uname) == "Linux" ]; then
    OS=linux-64
    echo "Linux OS"
    yum install -y wget git gcc
    wget --no-check https://repo.continuum.io/miniconda/Miniconda2-latest-Linux-x86_64.sh  -O miniconda2.sh 2> /dev/null
    bash miniconda2.sh -b -p $HOME/miniconda
    export PATH=$HOME/miniconda/bin:$PATH
    echo $PATH
    conda config --set always_yes yes --set changeps1 no
    conda update -y -q conda
    conda install libgfortran
    ln -s ~/miniconda/lib/libgfortran.so.3.0.0 ~/miniconda/envs/py2/lib/libgfortran.so
    ls -l ~/miniconda/lib
    export LD_LIBRARY_PATH=${HOME}/miniconda/lib
else
    echo "Mac OS"
    OS=osx-64
fi

mkdir ~/conda-bld
source activate root
conda install -q anaconda-client conda-build
conda config --set anaconda_upload no
export CONDA_BLD_PATH=${HOME}/conda-bld
export VERSION=$(date +%Y.%m.%d)
echo "Cloning recipes"
git clone git://github.com/UV-CDAT/conda-recipes
cd conda-recipes
# uvcdat creates issues for build -c uvcdat confises package and channel
rm -rf uvcdat
python ./prep_for_build.py -v $(date +%Y.%m.%d)
echo "Building and uploading now"
#
# use variant configuration in conda_build_config.yaml
#
conda build -c conda-forge  -m cdtime/conda_build_config.yaml ${PKG_NAME}  
#--numpy=1.13 --python=2.7
#conda build -c conda-forge  ${PKG_NAME} --numpy=1.12 --python=2.7
#conda build -c conda-forge  ${PKG_NAME} --numpy=1.11 --python=2.7
#conda build -c conda-forge  ${PKG_NAME} --numpy=1.10 --python=2.7
#conda build -c conda-forge  ${PKG_NAME} --numpy=1.9 --python=2.7
#conda build -c conda-forge  ${PKG_NAME} --numpy=1.13 --python=3.6
#conda build -c conda-forge  ${PKG_NAME} --numpy=1.12 --python=3.6
#conda build -c conda-forge  ${PKG_NAME} --numpy=1.11 --python=3.6
#conda build -c conda-forge  ${PKG_NAME} --numpy=1.10 --python=3.6
#conda build -c conda-forge  ${PKG_NAME} --numpy=1.9 --python=3.6
anaconda -t $CONDA_UPLOAD_TOKEN upload -u $USER -l nightly $CONDA_BLD_PATH/$OS/$PKG_NAME-$(date +%Y.%m.%d)-*_0.tar.bz2 --force
