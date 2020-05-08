#!/bin/bash

v=`clang-format --version | grep -Po '\d+.\d+.\d+'`

if [ `echo -e "$v\n10.0.0" | sort -V | head -n1` != '10.0.0' ]; then
    echo "clang-format version too old"
    exit;
fi

files=`ls src/* include/* test/* | grep ".cpp
.hpp"`

for f in $files; do
    `clang-format -style=file $f -i`
done