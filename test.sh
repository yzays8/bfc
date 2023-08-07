#!/bin/bash

readonly OUT="testc"

make runc FILE=$1 > $OUT.c
if [ $? -ne 0 ]; then
    echo "Failed to compile $1"
    exit 1
fi
gcc -o $OUT $OUT.c
./"$OUT"
