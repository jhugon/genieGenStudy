#!/bin/bash

N=100000

neutrinoFlux="1"
pionFlux="1"
energy=0.01,5.01

xsfile=/cvmfs/fermilab.opensciencegrid.org/products/larsoft/genie_xsec/v2_12_0/NULL/DefaultPlusMECWithNC/data/gxspl-FNALsmall.xml
messengerfile=$GENIE/config/Messenger_laconic.xml

# Hydrogen

gevgen -n $N -p 14 -t 1000010010 -e $energy --run 101 \
    -f $neutrinoFlux \
    --message-thresholds $messengerfile \
    --seed 2989819 --cross-sections $xsfile >& logGen101

mv gntp.0.ghep.root numH.ghep.root

gevgen -n $N -p -14 -t 1000010010 -e $energy --run 102 \
    -f $neutrinoFlux \
    --message-thresholds $messengerfile \
    --seed 2989819 --cross-sections $xsfile >& logGen102

mv gntp.0.ghep.root numbarH.ghep.root

gevgen -n $N -p 12 -t 1000010010 -e $energy --run 103 \
    -f $neutrinoFlux \
    --message-thresholds $messengerfile \
    --seed 2989819 --cross-sections $xsfile >& logGen103

mv gntp.0.ghep.root nueH.ghep.root

gevgen -n $N -p -12 -t 1000010010 -e $energy --run 104 \
    -f $neutrinoFlux \
    --message-thresholds $messengerfile \
    --seed 2989819 --cross-sections $xsfile >& logGen104

mv gntp.0.ghep.root nuebarH.ghep.root

gevgen_hadron -n $N -p 211 -t 1000010010 -k $energy --run 105 \
    -f $pionFlux \
    --message-thresholds $messengerfile \
    -m "hA" \
    --seed 2989819 --cross-sections $xsfile >& logGen105

mv gntp.inuke.0.ghep.root pipH.ghep.root

gevgen_hadron -n $N -p -211 -t 1000010010 -k $energy --run 106 \
    -f $pionFlux \
    --message-thresholds $messengerfile \
    -m "hA" \
    --seed 2989819 --cross-sections $xsfile >& logGen106

mv gntp.inuke.0.ghep.root pimH.ghep.root

# Argon 40 ### 1000180400

gevgen -n $N -p 14 -t 1000180400 -e $energy --run 111 \
    -f $neutrinoFlux \
    --message-thresholds $messengerfile \
    --seed 2989819 --cross-sections $xsfile >& logGen111

mv gntp.0.ghep.root numAr.ghep.root

gevgen -n $N -p -14 -t 1000180400 -e $energy --run 112 \
    -f $neutrinoFlux \
    --message-thresholds $messengerfile \
    --seed 2989819 --cross-sections $xsfile >& logGen112

mv gntp.0.ghep.root numbarAr.ghep.root

gevgen -n $N -p 12 -t 1000180400 -e $energy --run 113 \
    -f $neutrinoFlux \
    --message-thresholds $messengerfile \
    --seed 2989819 --cross-sections $xsfile >& logGen113

mv gntp.0.ghep.root nueAr.ghep.root

gevgen -n $N -p -12 -t 1000180400 -e $energy --run 114 \
    -f $neutrinoFlux \
    --message-thresholds $messengerfile \
    --seed 2989819 --cross-sections $xsfile >& logGen114

mv gntp.0.ghep.root nuebarAr.ghep.root

gevgen_hadron -n $N -p 211 -t 1000180400 -k $energy --run 115 \
    -f $pionFlux \
    --message-thresholds $messengerfile \
    -m "hA" \
    --seed 2989819 --cross-sections $xsfile >& logGen115

mv gntp.inuke.0.ghep.root pipAr.ghep.root

gevgen_hadron -n $N -p -211 -t 1000180400 -k $energy --run 116 \
    -f $pionFlux \
    --message-thresholds $messengerfile \
    -m "hA" \
    --seed 2989819 --cross-sections $xsfile >& logGen116

mv gntp.inuke.0.ghep.root pimAr.ghep.root

##### Make Ntuples

for f in *.ghep.root; do
  if [[ $f == "pi"* ]]
  then
    gntpc -i $f -f ginuke >& /dev/null
  else
    gntpc -i $f -f gst >& /dev/null
  fi
done
