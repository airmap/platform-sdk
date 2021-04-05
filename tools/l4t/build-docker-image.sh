#!/bin/bash

set -eux
HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

uname -r
sudo apt-get install qemu binfmt-support qemu-user-static

build_opts="--force-rm=true $@"
docker run --platform=linux/amd64 --rm --privileged multiarch/qemu-user-static --reset -p yes
sudo docker build ${build_opts} -t airmapd-l4t:latest -f $HERE/../../docker/l4t .
