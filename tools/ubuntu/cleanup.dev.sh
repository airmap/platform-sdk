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
  libssl-dev \
  pkg-config \
  qtbase5-dev

apt autoremove -y

# Now reinstall all our runtime dependencies
apt install -y libc-ares2 libssl1.0.0

# And remove all cached debian packages
apt clean
