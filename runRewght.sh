#!/bin/bash

##### Make Weight files

for f in *.ghep.root; do
  outf=${f/ghep/rewght}
  ./rewght -f $f --message-thresholds /home/centos/GENIE-Generator_v2.10.6/config/Messenger_laconic.xml >& /dev/null
  mv test.root $outf
done
