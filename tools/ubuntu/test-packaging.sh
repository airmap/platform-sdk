#!/bin/bash

set -ex
HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

#-- assumption that `make package` already ran and deposited the artifacts in the image's /tmp directory
find /tmp -maxdepth 1 -iname "airmap-platform-sdk*.deb" -exec dpkg -c {} +
