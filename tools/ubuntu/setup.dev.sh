#!/bin/bash

set -ex

apt update

apt install -y \
  build-essential \
  git \
  cmake \
  ninja-build \
  libssl-dev \
  pkg-config \
  qtbase5-dev \
  zlib1g-dev
