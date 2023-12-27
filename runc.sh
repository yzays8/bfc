#!/bin/bash

set -eu
cd $(dirname $0)

readonly OUT="testc"

make runc FILE=$1 > $OUT.c
gcc -o $OUT $OUT.c
./"$OUT"
