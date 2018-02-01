#!/usr/bin/env bash
export UVCDAT_ANONYMOUS_LOG=False
export PATH=${HOME}/miniconda/bin:${PATH}
source activate py2
python run_tests.py -v2 
RESULT=$?
source activate py3
python run_tests.py -v2 
RESULT=$(( $RESULT + $? ))
echo "RESULT:"${RESULT}
if [ $RESULT -eq 0 -a $CIRCLE_BRANCH == "master" ]; then LABEL=nightly bash ./ci-support/conda_upload.sh ; fi
if [ $RESULT -eq 0 -a $CIRCLE_BRANCH != "master" ]; then LABEL=unstable bash ./ci-support/conda_upload.sh ; fi
exit $RESULT
