#!/bin/bash

FILES="*.txt"
for f in $FILES
do
  echo "Processing file $f ..."
  xxd -i $f > "../source/test/cpp/data/input_${f/.txt/.cpp}"
done
