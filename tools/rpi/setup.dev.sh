#!/bin/bash

set -ex

apt update

apt install -y \
  build-essential \
  cmake \
  git \
  golang \
  ninja-build \
  libssl-dev \
  perl \
  pkg-config \
  qtbase5-dev \
  zlib1g-dev
