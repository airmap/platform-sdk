#!/bin/bash

set -ex

apt update

apt install -y \
  build-essential \
  cmake \
  ninja-build \
  libboost-date-time1.63-dev \
  libboost-filesystem1.63-dev \
  libboost-program-options1.63-dev \
  libboost-system1.63-dev \
  libprotobuf-dev \
  libssl-dev \
  pkg-config \
  protobuf-compiler \
  qtbase5-dev
