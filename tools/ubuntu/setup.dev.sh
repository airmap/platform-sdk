#!/bin/bash

set -ex

apt update

apt install -y \
  build-essential \
  cmake \
  libboost-date-time1.63-dev \
  libboost-program-options1.63-dev \
  libboost-system1.63-dev \
  libprotobuf-dev \
  libssl-dev \
  pkg-config \
  protobuf-compiler
