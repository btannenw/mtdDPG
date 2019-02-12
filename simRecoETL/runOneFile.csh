#!/bin/bash
source /cvmfs/sft.cern.ch/lcg/views/LCG_89/x86_64-slc6-gcc62-opt/setup.sh
cd ${_CONDOR_SCRATCH_DIR}
#xrdcp -s root://cmseos.fnal.gov//store/user/benjtann/$2/$7 .
tar -xf $7
rm $7

python quickETLPerformance.py --input=${1} --output=${2}/${2}_${3}.root --layout=${4} --events=${5} --firstEvent=${6}

### Now that the run is over, there is one or more root files created
echo "List all root files = "
ls $2/*.root
echo "List all files"
ls 
echo "*******************************************"
OUTDIR=root://cmseos.fnal.gov//store/user/benjtann/$2/
echo "xrdcp output for condor"
for FILE in $2/*.root
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
