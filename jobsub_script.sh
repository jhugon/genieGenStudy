#!/bin/bash

# submit with -dOUT /pnfs/lariat/scratch/users/jhugon/genie/test2

echo "starting gevgen job"

cd $TEMP
echo "Hostname:"
hostname
echo "PROBEID: $PROBEID"
echo "TARGETID: $TARGETID"
echo "PROCESS: $PROCESS"
echo "CWD:"
pwd

source /cvmfs/fermilab.opensciencegrid.org/products/larsoft/setups
version=v06_34_01
qual=e14:prof
setup larsoft $version -q $qual

N=1000

pionFlux="1"
energy=0.01,2.01

xsfile=/cvmfs/fermilab.opensciencegrid.org/products/larsoft/genie_xsec/v2_12_0/NULL/DefaultPlusMECWithNC/data/gxspl-FNALsmall.xml
messengerfile=$GENIE/config/Messenger_laconic.xml

echo "Staring gevgen:"

gevgen_hadron -n $N -p $PROBEID -t $TARGETID -k $energy --run $PROCESS \
    -f $pionFlux \
    --message-thresholds $messengerfile \
    -m "hA" \
    --seed 2989819 --cross-sections $xsfile >& logGen

echo "Staring gntpc:"

for f in *.ghep.root; do
  gntpc -i $f -f ginuke >& /dev/null
done

echo "ls CWD:"
ls -lhtr

outdir=$CONDOR_DIR_OUT/$PROCESS
echo "CONDOR_DIR_OUT: $CONDOR_DIR_OUT"
echo "out subdir: $outdir"
echo "making out subdir"
mkdir -p $outdir
echo "copying to out subdir"
cp * $outdir

echo "ls CONDOR_DIR_OUT:"
ls -lhtr $CONDOR_DIR_OUT
echo "ls $outdir:"
ls -lhtr $outdir

echo "done!"
