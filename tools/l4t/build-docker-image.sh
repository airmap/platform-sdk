#!/bin/bash

set -eux
HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

uname -r
sudo apt-get install qemu

build_opts="--force-rm=true $@"
sudo docker run --platform=linux/amd64 --rm --privileged multiarch/qemu-user-static:register || true
#git clone https://github.com/computermouth/qemu-static-conf.git
#sudo mkdir -p /usr/lib/binfmt.d
#sudo cp qemu-static-conf/*.conf /usr/lib/binfmt.d/
#sudo systemctl restart systemd-binfmt.service || true
sudo systemctl status systemd-binfmt || true
journalctl -b -g binfmt || true
sudo docker build ${build_opts} --platform=linux/arm64 -t airmapd-l4t:latest -f $HERE/../../docker/l4t .
sudo docker run --platform=linux/amd64 --rm --privileged multiarch/qemu-user-static:register --reset -p yes
