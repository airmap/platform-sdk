#!/bin/ash

set -ex

apk update

apk add libressl2.5-libssl \
    boost-system \
    boost-program_options \
    boost-date_time \
    boost-filesystem \
    zlib

apk add build-base \
    cmake \
    git \
    ninja \
    pkgconfig \
    boost-dev \
    libressl-dev \
    qt5-qtbase-dev
