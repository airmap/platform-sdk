#!/bin/bash

set -ex
HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

#-- assumption that `make package` already ran and deposited the artifacts in the build directory
#RUN find . -maxdepth 1 -name "airmap-platform-sdk*.deb" -exec dpkg -c {} +
