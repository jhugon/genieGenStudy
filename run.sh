#!/bin/bash

N=100000

# Hydrogen

nohup nice gevgen -n $N -p 14 -t 1000010010 -e 0.1,20 --run 101 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen101

mv gntp.0.ghep.root numH.ghep.root

nohup nice gevgen -n $N -p -14 -t 1000010010 -e 0.1,20 --run 102 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen102

mv gntp.0.ghep.root numbarH.ghep.root

nohup nice gevgen -n $N -p 12 -t 1000010010 -e 0.1,20 --run 103 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen103

mv gntp.0.ghep.root nueH.ghep.root

nohup nice gevgen -n $N -p -12 -t 1000010010 -e 0.1,20 --run 104 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen104

mv gntp.0.ghep.root nuebarH.ghep.root

nohup nice gevgen_hadron -n $N -p 211 -t 1000010010 -k 0.1,20 --run 105 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    -m "hA" \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen105

mv gntp.inuke.0.ghep.root pipH.ghep.root

nohup nice gevgen_hadron -n $N -p -211 -t 1000010010 -k 0.1,20 --run 106 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    -m "hA" \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen106

mv gntp.inuke.0.ghep.root pimH.ghep.root

# Argon 40 ### 1000180400

nohup nice gevgen -n $N -p 14 -t 1000180400 -e 0.1,20 --run 111 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen111

mv gntp.0.ghep.root numAr.ghep.root

nohup nice gevgen -n $N -p -14 -t 1000180400 -e 0.1,20 --run 112 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen112

mv gntp.0.ghep.root numbarAr.ghep.root

nohup nice gevgen -n $N -p 12 -t 1000180400 -e 0.1,20 --run 113 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen113

mv gntp.0.ghep.root nueAr.ghep.root

nohup nice gevgen -n $N -p -12 -t 1000180400 -e 0.1,20 --run 114 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen114

mv gntp.0.ghep.root nuebarAr.ghep.root

nohup nice gevgen_hadron -n $N -p 211 -t 1000180400 -k 0.1,20 --run 115 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    -m "hA" \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen115

mv gntp.inuke.0.ghep.root pipAr.ghep.root

nohup nice gevgen_hadron -n $N -p -211 -t 1000180400 -k 0.1,20 --run 116 \
    -f "1" \
    --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml \
    -m "hA" \
    --seed 2989819 --cross-sections /home/centos/genie_xs_data/gxspl-HAr.xml >& logGen116

mv gntp.inuke.0.ghep.root pimAr.ghep.root

##### Make Ntuples

for f in *.ghep.root; do
  if [[ $f == "pi"* ]]
  then
    nohup nice gntpc -i $f -f ginuke >& /dev/null
  else
    nohup nice gntpc -i $f -f gst >& /dev/null
  fi
done
