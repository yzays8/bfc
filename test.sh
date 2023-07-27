#!/bin/bash

readonly OUT="testc"

make run FILE=$1 > $OUT.c
gcc -o $OUT $OUT.c
./"$OUT"
