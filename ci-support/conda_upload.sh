#!/usr/bin/env bash
PKG_NAME=cdtime
USER=uvcdat
export PATH="$HOME/miniconda/bin:$PATH"
echo "Trying to upload conda"
if [ `uname` == "Linux" ]; then
    OS=linux-64
    echo "Linux OS"
    yum install -y wget git gcc
    # wget --no-check https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh  -O miniconda3.sh 2> /dev/null
    wget --no-check https://repo.continuum.io/miniconda/Miniconda2-4.3.30-Linux-x86_64.sh  -O miniconda2.sh 2> /dev/null
    bash miniconda2.sh -b -p ${HOME}/miniconda
    export SYSPATH=$PATH
    export PATH=${HOME}/miniconda/bin:${SYSPATH}
    echo $PATH
    conda config --set always_yes yes --set changeps1 no
    conda config --set anaconda_upload false --set ssl_verify false
    conda install -n root -q anaconda-client "conda-build<3.3"
    conda install -n root gcc future
    which python
    export UVCDAT_ANONYMOUS_LOG=False
else
    echo "Mac OS"
    OS=osx-64
fi

mkdir ~/conda-bld
source activate root
if [ `uname` == "Linux" ]; then
    conda install -n root -q anaconda-client "conda-build<3.3"
else
    conda install -n root -q anaconda-client conda-build
fi
conda config --set anaconda_upload no
export CONDA_BLD_PATH=${HOME}/conda-bld
export VERSION=$(date +%Y.%m.%d)
echo "Cloning recipes"
git clone git://github.com/UV-CDAT/conda-recipes
cd conda-recipes
# uvcdat creates issues for build -c uvcdat confises package and channel
rm -rf uvcdat
python ./prep_for_build.py -v ${VERSION}
#
echo "Building now"
conda build -c conda-forge  ${PKG_NAME} 
echo "Uploading now"
mkdir -p ~/.continuum/anaconda-client/
echo "ssl_verify: false" >> ~/.continuum/anaconda-client/config.yaml
echo "verify_ssl: false" >> ~/.continuum/anaconda-client/config.yaml
anaconda -t $CONDA_UPLOAD_TOKEN upload -u $USER -l ${LABEL} $CONDA_BLD_PATH/$OS/$PKG_NAME-$(date +%Y.%m.%d)-*_0.tar.bz2 --force
