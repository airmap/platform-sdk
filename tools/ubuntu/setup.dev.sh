#!/bin/bash

set -ex

apt update

apt install -y \
  build-essential \
  curl \
  git \
  golang \
  cmake \
  ninja-build \
  libssl-dev \
  perl \
  pkg-config \
  qtbase5-dev \
  unzip \
  zlib1g-dev
