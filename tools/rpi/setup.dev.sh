#!/bin/bash

set -ex

apt update

apt install -y \
  build-essential \
  cmake \
  git \
  ninja-build \
  libboost-date-time1.62-dev \
  libboost-filesystem1.62-dev \
  libboost-program-options1.62-dev \
  libboost-system1.62-dev \
  libboost-test1.62-dev \
  libssl-dev \
  pkg-config \
  qtbase5-dev \
  zlib1g-dev
