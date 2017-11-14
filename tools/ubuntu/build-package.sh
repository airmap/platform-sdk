#!/bin/sh

set -e

apt update
# TODO(tvoss): We should host the protobuf3 packages in a PPA we own.
apt install software-properties-common
add-apt-repository -y ppa:maarten-fonville/protobuf
apt update
apt build-dep -y ./
apt upgrade -y
dpkg-buildpackage -j4
