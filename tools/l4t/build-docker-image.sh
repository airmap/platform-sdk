#!/bin/bash

set -eux
HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

build_opts="--force-rm=true $@"
docker run --platform=linux/amd64 --rm --privileged multiarch/qemu-user-static:register || true
systemctl status systemd-binfmt
docker build ${build_opts} --platform=linux/arm64 -t airmapd-l4t:latest -f $HERE/../../docker/l4t .
docker run --platform=linux/amd64 --rm --privileged multiarch/qemu-user-static:register --reset -p yes
