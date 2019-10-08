#!/bin/bash

set -ex

build_opts="--force-rm=true $@"

docker build ${build_opts} -t centos/airmapd:latest -f docker/centos .
