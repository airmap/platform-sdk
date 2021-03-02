#!/bin/bash

set -eo pipefail

IMAGE=${1:-platform-sdk.ubuntu}
ARTIFACT_DIR="artifact_dir"

mkdir -p ./$ARTIFACT_DIR

DEBS=$(docker run -i -a stdout $IMAGE /bin/bash -c "find . -name airmap-platform-sdk*.deb")
CONTAINER=$(docker create $IMAGE)
for deb in $DEBS; do
	docker cp $CONTAINER:$deb ./$ARTIFACT_DIR
done