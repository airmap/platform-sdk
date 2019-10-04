#!/bin/bash

set -ex

build_opts="--force-rm=true $@"

sudo docker build ${build_opts} -t airmapd:latest -f docker/centos .
