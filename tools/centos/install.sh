#!/bin/bash
set -ex

if [ "$#" -ne 1 ]; then
    echo "Provide installation folder"
    exit 1
fi

mkdir -p $1
mkdir -p $1/lib

CONTAINER=$(docker create centos/airmapd:latest)
docker cp $CONTAINER:/usr/include/airmap ./$1/airmap
docker cp $CONTAINER:/usr/lib64/libairmap-cpp.so ./$1/lib
docker cp $CONTAINER:/usr/lib64/libairmap-cpp.so.0.0.1 ./$1/lib
docker cp $CONTAINER:/usr/lib64/libairmap-qt.so.0 ./$1/lib
docker cp $CONTAINER:/usr/lib64/libairmap-cpp.so.0 ./$1/lib
docker cp $CONTAINER:/usr/lib64/libairmap-qt.so ./$1/lib
docker cp $CONTAINER:/usr/lib64/libairmap-qt.so.0.0.1 ./$1/lib
