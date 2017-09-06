#!/bin/bash

set -ex

# Please note that --squash=true requires a docker daemon that runs
# with the experimental flag set to true.
sudo docker build --force-rm=true --squash=true -t airmapd:latest -f Dockerfile.alpine .
