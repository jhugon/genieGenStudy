#!/bin/bash

# submit with -dOUT /pnfs/lariat/scratch/users/jhugon/genie/test2

echo "starting gevgen job"

cd $TEMP
echo "Hostname:"
hostname
echo "CWD:"
pwd

source /cvmfs/fermilab.opensciencegrid.org/products/larsoft/setups
version=v06_34_01
qual=e14:prof
setup larsoft $version -q $qual

N=1000000

pionFlux="1"
energy=0.01,2.01

xsfile=/cvmfs/fermilab.opensciencegrid.org/products/larsoft/genie_xsec/v2_12_0/NULL/DefaultPlusMECWithNC/data/gxspl-FNALsmall.xml
messengerfile=$GENIE/config/Messenger_laconic.xml

echo "Staring gevgen:"

gevgen_hadron -n $N -p -211 -t 1000180400 -k $energy --run 116 \
    -f $pionFlux \
    --message-thresholds $messengerfile \
    -m "hA" \
    --seed 2989819 --cross-sections $xsfile >& logGen116

mv gntp.inuke.0.ghep.root pimAr.ghep.root

echo "Staring gntpc:"

for f in *.ghep.root; do
  if [[ $f == "pi"* ]]
  then
    gntpc -i $f -f ginuke >& /dev/null
  else
    gntpc -i $f -f gst >& /dev/null
  fi
done

echo "ls CWD:"
ls -lhtr

echo "CONDOR_DIR_OUT: $CONDOR_DIR_OUT"
echo "copying to CONDOR_DIR_OUT:"
cp * $CONDOR_DIR_OUT

echo "ls CONDOR_DIR_OUT:"
ls -lhtr $CONDOR_DIR_OUT

echo "done!"
