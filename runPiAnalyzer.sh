#!/bin/bash

#for f in *.ginuke.root; do
#  command="root -b -q -x piAnalyzer.C+(\"$f\")"
#  echo $command
#  $command
#done


command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/test3/pimAr.ginuke.root\",\"pimAr.hists.root\")"
echo $command
$command
