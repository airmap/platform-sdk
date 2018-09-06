#!/bin/ash

set -ex

apk update

apk add libressl zlib

apk add build-base \
    cmake \
    git \
    go \
    ninja \
    perl \
    pkgconfig \
    libressl-dev \
    qt5-qtbase-dev
