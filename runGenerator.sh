#!/bin/bash

N=1000000

neutrinoFlux="1"
pionFlux="1"
energy=0.01,5.01

# Hydrogen

gevgen -n $N -p 14 -t 1000010010 -e $energy --run 101 \
    -f $neutrinoFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen101

mv gntp.0.ghep.root numH.ghep.root

gevgen -n $N -p -14 -t 1000010010 -e $energy --run 102 \
    -f $neutrinoFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen102

mv gntp.0.ghep.root numbarH.ghep.root

gevgen -n $N -p 12 -t 1000010010 -e $energy --run 103 \
    -f $neutrinoFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen103

mv gntp.0.ghep.root nueH.ghep.root

gevgen -n $N -p -12 -t 1000010010 -e $energy --run 104 \
    -f $neutrinoFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen104

mv gntp.0.ghep.root nuebarH.ghep.root

gevgen_hadron -n $N -p 211 -t 1000010010 -k $energy --run 105 \
    -f $pionFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    -m "hA" \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen105

mv gntp.inuke.0.ghep.root pipH.ghep.root

gevgen_hadron -n $N -p -211 -t 1000010010 -k $energy --run 106 \
    -f $pionFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    -m "hA" \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen106

mv gntp.inuke.0.ghep.root pimH.ghep.root

# Argon 40 ### 1000180400

gevgen -n $N -p 14 -t 1000180400 -e $energy --run 111 \
    -f $neutrinoFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen111

mv gntp.0.ghep.root numAr.ghep.root

gevgen -n $N -p -14 -t 1000180400 -e $energy --run 112 \
    -f $neutrinoFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen112

mv gntp.0.ghep.root numbarAr.ghep.root

gevgen -n $N -p 12 -t 1000180400 -e $energy --run 113 \
    -f $neutrinoFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen113

mv gntp.0.ghep.root nueAr.ghep.root

gevgen -n $N -p -12 -t 1000180400 -e $energy --run 114 \
    -f $neutrinoFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen114

mv gntp.0.ghep.root nuebarAr.ghep.root

gevgen_hadron -n $N -p 211 -t 1000180400 -k $energy --run 115 \
    -f $pionFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    -m "hA" \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen115

mv gntp.inuke.0.ghep.root pipAr.ghep.root

gevgen_hadron -n $N -p -211 -t 1000180400 -k $energy --run 116 \
    -f $pionFlux \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    -m "hA" \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen116

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
