#!/bin/bash

set -ex

apt update

apt install -y \
  build-essential \
  git \
  cmake \
  ninja-build \
  libboost-date-time1.63-dev \
  libboost-filesystem1.63-dev \
  libboost-program-options1.63-dev \
  libboost-system1.63-dev \
  libboost-test1.63-dev \
  libc-ares-dev \
  libssl-dev \
  pkg-config \
  qtbase5-dev \
  zlib1g-dev
