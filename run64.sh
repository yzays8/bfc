#!/bin/bash

set -eu
cd $(dirname $0)

readonly OUT="test64"

make run FILE=$1 > $OUT.s
gcc -static $OUT.s -o $OUT
./"$OUT"
