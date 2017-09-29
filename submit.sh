#!/bin/bash

script="file://jobsub_script.sh"
joblifetime=3h

export PROBEID="211"
export TARGETID="1000180400"
outdir=/pnfs/lariat/scratch/users/jhugon/genie/genie_"$PROBEID"_Ar40_v1
command="jobsub_submit --expected-lifetime=$joblifetime -dOUT $outdir -e PROBEID -e TARGETID $script"
echo $command
$command

export PROBEID="-211"
export TARGETID="1000180400"
outdir=/pnfs/lariat/scratch/users/jhugon/genie/genie_"$PROBEID"_Ar40_v1
command="jobsub_submit --expected-lifetime=$joblifetime -dOUT $outdir -e PROBEID -e TARGETID $script"
echo $command
#$command

export PROBEID="321"
export TARGETID="1000180400"
outdir=/pnfs/lariat/scratch/users/jhugon/genie/genie_"$PROBEID"_Ar40_v1
command="jobsub_submit --expected-lifetime=$joblifetime -dOUT $outdir -e PROBEID -e TARGETID $script"
echo $command
#$command

export PROBEID="-321"
export TARGETID="1000180400"
outdir=/pnfs/lariat/scratch/users/jhugon/genie/genie_"$PROBEID"_Ar40_v1
command="jobsub_submit --expected-lifetime=$joblifetime -dOUT $outdir -e PROBEID -e TARGETID $script"
echo $command
#$command

export PROBEID="2212"
export TARGETID="1000180400"
outdir=/pnfs/lariat/scratch/users/jhugon/genie/genie_"$PROBEID"_Ar40_v1
command="jobsub_submit --expected-lifetime=$joblifetime -dOUT $outdir -e PROBEID -e TARGETID $script"
echo $command
#$command

