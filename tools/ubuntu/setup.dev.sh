#!/bin/bash

set -ex

apt update

apt install -y \
  build-essential \
  git \
  golang \
  cmake \
  ninja-build \
  libssl-dev \
  perl \
  pkg-config \
  qtbase5-dev \
  zlib1g-dev
