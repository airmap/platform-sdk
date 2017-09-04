#!/bin/bash

set -ex

brew update

brew install \
  cmake \
  boost \
  pkgconfig \
  protobuf \
  openssl || true

ln -s /usr/local/opt/openssl/include/openssl/ /usr/local/include/
ln -s /usr/local/opt/openssl/lib/libcrypto.1.0.0.dylib /usr/local/lib/
ln -s /usr/local/opt/openssl/lib/libssl.1.0.0.dylib /usr/local/lib/
ln -s /usr/local/opt/openssl/lib/pkgconfig/libssl.pc /usr/local/lib/pkgconfig
ln -s /usr/local/opt/openssl/lib/pkgconfig/libcrypto.pc /usr/local/lib/pkgconfig
ln -s /usr/local/opt/openssl/lib/pkgconfig/openssl.pc /usr/local/lib/pkgconfig
