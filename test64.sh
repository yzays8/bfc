#!/bin/bash

readonly OUT="test64"

make run FILE=$1 > $OUT.s
gcc -static $OUT.s -o $OUT
./"$OUT"
