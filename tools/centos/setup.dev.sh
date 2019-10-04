#!/bin/bash

# This setup is considered experimental thus far.

set -ex

yum -y update

# We first satisfy all of our basic build-dependencies
# before diving deeper and pulling in required build-time
# components.
yum install -y \
  curl \
  git \
  patch \
  perl \
  pkg-config \
  qtbase5-dev \
  unzip \
  zlib1g-dev

# Please see https://www.softwarecollections.org/en/scls/rhscl/devtoolset-7/
# for further details.
yum install -y centos-release-scl
yum install -y devtoolset-7

# Download the latest CMake release
curl -L https://github.com/Kitware/CMake/releases/download/v3.14.4/cmake-3.14.4-Linux-x86_64.tar.gz > /usr/local/cmake-3.14.4-Linux-x86_64.tar.gz
tar -C /usr/local -xf /usr/local/cmake-3.14.4-Linux-x86_64.tar.gz

# Download the latest golang release
curl https://dl.google.com/go/go1.12.5.linux-amd64.tar.gz > /usr/local/go1.12.5.linux-amd64.tar.gz
tar -C /usr/local -xf /usr/local/go1.12.5.linux-amd64.tar.gz
