#!/bin/bash

for f in *.ginuke.root; do
  command="root -b -q -x piAnalyzer.C+(\"$f\")"
  echo $command
  $command
done
