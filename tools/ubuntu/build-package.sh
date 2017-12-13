#!/bin/sh

set -e

apt update
apt --yes build-dep ./
dpkg-buildpackage -j4
