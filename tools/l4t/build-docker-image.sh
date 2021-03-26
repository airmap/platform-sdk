#!/bin/bash

set -exo
HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
echo $HERE

build_opts="--force-rm=true $@"
sudo docker run --rm --privileged multiarch/qemu-user-static:register || true
sudo docker build ${build_opts} -t airmapd-l4t:latest -f $HERE/../../docker/l4t . 
sudo docker run --rm --privileged multiarch/qemu-user-static:register --reset
