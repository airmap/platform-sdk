#!/bin/bash

set -ex
HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

apt update

apt install -y \
  build-essential \
  curl \
  git \
  golang \
  libssl-dev \
  perl \
  pkg-config \
  zlib1g-dev

apt purge -y --auto-remove cmake
$HERE/../build.cmake.sh
