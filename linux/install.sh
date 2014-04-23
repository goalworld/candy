#!/bin/bash
cp ./build/libcandy.so /usr/lib
if [ -x "/usr/lib64" ]; then
cp ./build/libcandy.so /usr/lib64
fi

mkdir -p /usr/include/candy
cp -r ../include/candy/*  /usr/include/candy/