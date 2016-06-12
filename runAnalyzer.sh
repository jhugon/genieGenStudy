#!/bin/bash

for f in *.gst.root; do
  #command="root -b -q -x analyzer.C+\(\"$f\"\)"
  command="root -b -q -x analyzer.C+(\"$f\")"
  echo $command
  $command
done
