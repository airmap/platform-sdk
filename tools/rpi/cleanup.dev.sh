#!/bin/bash
set -ex

apt update

# Let's remove our build dependencies
# and be quite aggressive about it.
apt remove -y \
  build-essential \
  cmake \
  git \
  ninja-build \
  libboost-date-time1.62-dev \
  libboost-filesystem1.62-dev \
  libboost-program-options1.62-dev \
  libboost-system1.62-dev \
  libboost-test1.62-dev \
  libc-ares-dev \
  libssl-dev \
  pkg-config \
  qtbase5-dev \
  zlib1g-dev

apt autoremove -y

# Now reinstall all our runtime dependencies
apt install -y \
  libboost-date-time1.62.0 \
  libboost-filesystem1.62.0 \
  libboost-program-options1.62.0 \
  libboost-system1.62.0 \
  libprotobuf10 \
  libssl1.1

# And remove all cached debian packages
apt clean
