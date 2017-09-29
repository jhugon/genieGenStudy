#!/bin/bash

script="file://jobsub_script.sh"
joblifetime=3h

for isAr40 in 0 1; do

  if [[ $isAr40 == 1 ]]; then
    export TARGETID="1000180400"
    targetstr="Ar40"
  else
    export TARGETID="1000010010"
    targetstr="H1"
  fi
  
  export PROBEID="211"
  outdir=/pnfs/lariat/scratch/users/jhugon/genie/genie_"$PROBEID"_"$targetstr"_v2
  command="jobsub_submit --expected-lifetime=$joblifetime -dOUT $outdir -e PROBEID -e TARGETID $script"
  echo $command
  $command
  
  export PROBEID="-211"
  outdir=/pnfs/lariat/scratch/users/jhugon/genie/genie_"$PROBEID"_"$targetstr"_v2
  command="jobsub_submit --expected-lifetime=$joblifetime -dOUT $outdir -e PROBEID -e TARGETID $script"
  echo $command
  $command
  
  export PROBEID="321"
  outdir=/pnfs/lariat/scratch/users/jhugon/genie/genie_"$PROBEID"_"$targetstr"_v2
  command="jobsub_submit --expected-lifetime=$joblifetime -dOUT $outdir -e PROBEID -e TARGETID $script"
  echo $command
  $command
  
  export PROBEID="-321"
  outdir=/pnfs/lariat/scratch/users/jhugon/genie/genie_"$PROBEID"_"$targetstr"_v2
  command="jobsub_submit --expected-lifetime=$joblifetime -dOUT $outdir -e PROBEID -e TARGETID $script"
  echo $command
  $command
  
  export PROBEID="2212"
  outdir=/pnfs/lariat/scratch/users/jhugon/genie/genie_"$PROBEID"_"$targetstr"_v2
  command="jobsub_submit --expected-lifetime=$joblifetime -dOUT $outdir -e PROBEID -e TARGETID $script"
  echo $command
  $command
  
done
