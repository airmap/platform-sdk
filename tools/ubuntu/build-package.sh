#!/bin/sh

set -e

apt update
# TODO(tvoss): We should host the protobuf3 packages in a PPA we own.
apt --yes --force-yes install software-properties-common
add-apt-repository -y ppa:maarten-fonville/protobuf
apt update
apt --yes build-dep ./
apt --yes --force-yes upgrade
dpkg-buildpackage -j4
