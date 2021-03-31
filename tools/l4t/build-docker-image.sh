#!/bin/bash

set -eux
HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

build_opts="--force-rm=true $@"
docker run --rm --privileged multiarch/qemu-user-static:register || true
docker build ${build_opts} -t airmapd-l4t:latest -f $HERE/../../docker/l4t .
docker run --rm --privileged multiarch/qemu-user-static:register --reset -p yes
