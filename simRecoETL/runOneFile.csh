#!/bin/bash
source /cvmfs/sft.cern.ch/lcg/views/LCG_89/x86_64-slc6-gcc62-opt/setup.sh
cd ${_CONDOR_SCRATCH_DIR}
xrdcp -s root://cmseos.fnal.gov//store/user/benjtann/$3/$6 .
tar -xf $6
rm $6

python drawETLPerformance.py --input=${1} --output=${3}/${3}_${4}.root --layout=${5}

### Now that the run is over, there is one or more root files created
echo "List all root files = "
ls $3/*.root
echo "List all files"
ls 
echo "*******************************************"
OUTDIR=root://cmseos.fnal.gov//store/user/benjtann/$3/
echo "xrdcp output for condor"
for FILE in $3/*.root
do
  echo "xrdcp -f ${FILE} ${OUTDIR}/${FILE}"
  xrdcp -f ${FILE} ${OUTDIR}/${FILE} 2>&1
  XRDEXIT=$?
  if [[ $XRDEXIT -ne 0 ]]; then
    rm *.root
    echo "exit code $XRDEXIT, failure in xrdcp"
    exit $XRDEXIT
  fi
  rm ${FILE}
done
