#!/bin/bash

#for f in *.ginuke.root; do
#  command="root -b -q -x piAnalyzer.C+(\"$f\")"
#  echo $command
#  $command
#done


#command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/test3/pimAr.ginuke.root\",\"pimAr.hists.root\")"
#echo $command
#$command
command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/genie_-211_Ar40_v3/gntp.inuke.0.ginuke.root\",\"pimAr.hists.root\")"
echo $command
$command
command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/genie_211_Ar40_v3/gntp.inuke.0.ginuke.root\",\"pipAr.hists.root\")"
echo $command
$command
command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/genie_-321_Ar40_v3/gntp.inuke.0.ginuke.root\",\"kmAr.hists.root\")"
echo $command
$command
command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/genie_321_Ar40_v3/gntp.inuke.0.ginuke.root\",\"kpAr.hists.root\")"
echo $command
$command
command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/genie_2212_Ar40_v3/gntp.inuke.0.ginuke.root\",\"pAr.hists.root\")"
echo $command
$command
command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/genie_-211_H1_v3/gntp.inuke.0.ginuke.root\",\"pimH.hists.root\")"
echo $command
$command
command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/genie_211_H1_v3/gntp.inuke.0.ginuke.root\",\"pipH.hists.root\")"
echo $command
$command
command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/genie_-321_H1_v3/gntp.inuke.0.ginuke.root\",\"kmH.hists.root\")"
echo $command
$command
command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/genie_321_H1_v3/gntp.inuke.0.ginuke.root\",\"kpH.hists.root\")"
echo $command
$command
command="root -b -q -x piAnalyzer.C+(\"/pnfs/lariat/scratch/users/jhugon/genie/genie_2212_H1_v3/gntp.inuke.0.ginuke.root\",\"pH.hists.root\")"
echo $command
$command
