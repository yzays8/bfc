#!/bin/bash

readonly OUT="test64"

make run FILE=$1 > $OUT.s
if [ $? -ne 0 ]; then
    echo "Failed to compile $1"
    exit 1
fi
gcc -static $OUT.s -o $OUT
./"$OUT"
