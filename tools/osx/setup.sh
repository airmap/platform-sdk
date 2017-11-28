#!/bin/bash

set -ex

brew update

brew install \
  c-ares \
  cmake \
  boost \
  pkgconfig \
  openssl \
  qt \
  zlib || true

ln -s /usr/local/opt/openssl/include/openssl/ /usr/local/include/ || true
ln -s /usr/local/opt/openssl/lib/libcrypto.1.0.0.dylib /usr/local/lib/ || true
ln -s /usr/local/opt/openssl/lib/libssl.1.0.0.dylib /usr/local/lib/ || true
ln -s /usr/local/opt/openssl/lib/pkgconfig/libssl.pc /usr/local/lib/pkgconfig || true
ln -s /usr/local/opt/openssl/lib/pkgconfig/libcrypto.pc /usr/local/lib/pkgconfig || true
ln -s /usr/local/opt/openssl/lib/pkgconfig/openssl.pc /usr/local/lib/pkgconfig || true
