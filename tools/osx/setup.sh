#!/bin/bash

set -ex

brew update

brew install \
  cmake \
  go \
  perl \
  pkgconfig \
  qt \
  zlib || true
