#!/bin/bash

set -ex

apt update

apt install -y \
  build-essential \
  cmake \
  ninja-build \
  libboost-date-time1.62-dev \
  libboost-filesystem1.62-dev \
  libboost-program-options1.62-dev \
  libboost-system1.62-dev \
  libboost-test1.62-dev \
  libc-ares-dev \
  libssl-dev \
  pkg-config \
  zlib1g-dev
