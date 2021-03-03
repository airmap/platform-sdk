#!/bin/bash

set -euxo pipefail

IMAGE=${1:-platform-sdk.ubuntu}
ARTIFACT_DIR="artifact_dir"

mkdir -p ./$ARTIFACT_DIR

CONTAINER=$(docker create $IMAGE)
DEBS=$(docker run -i -a stdout $IMAGE /bin/bash -c "find . -name airmap-platform-sdk*.deb")
for deb in $DEBS; do
	docker cp $CONTAINER:$deb ./$ARTIFACT_DIR
done