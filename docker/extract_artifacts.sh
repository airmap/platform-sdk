#!/bin/bash

set -euxo pipefail

IMAGE=${1:-platform-sdk.ubuntu}
ARTIFACT_DIR=$2

mkdir -p $ARTIFACT_DIR

CONTAINER=$(docker create $IMAGE)
DEBS=$(docker run -i -a stdout $IMAGE /bin/bash -c "find /usr/src/app -name airmap-platform-sdk*.deb")
for deb in $DEBS; do
	docker cp $CONTAINER:$deb $ARTIFACT_DIR
done