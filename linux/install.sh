#!/bin/bash
cp ./build/libcandy.so /usr/lib
mkdir -p /usr/include/candy
cp -r ../include/candy/*  /usr/include