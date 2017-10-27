#!/bin/bash
set -ex

apt update

# Let's remove our build dependencies
# and be quite aggressive about it.
apt remove -y \
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

apt autoremove -y

# Now reinstall all our runtime dependencies
apt install -y \
  libboost-date-time1.63.0 \
  libboost-filesystem1.63.0 \
  libboost-program-options1.63.0 \
  libboost-system1.63.0 \
  libprotobuf10 \
  libssl1.0.0

# And remove all cached debian packages
apt clean
