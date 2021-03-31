#!/bin/bash

set -eux
HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

build_opts="--force-rm=true $@"
docker run --rm --privileged multiarch/qemu-user-static:register || true
git clone https://github.com/computermouth/qemu-static-conf.git
sudo mkdir -p /lib/binfmt.d
sudo cp qemu-static-conf/*.conf /lib/binfmt.d/
sudo systemctl restart systemd-binfmt.service
systemctl status systemd-binfmt.service
journalctl -xe
docker build ${build_opts} -t airmapd-l4t:latest -f $HERE/../../docker/l4t .
docker run --rm --privileged multiarch/qemu-user-static:register --reset -p yes
