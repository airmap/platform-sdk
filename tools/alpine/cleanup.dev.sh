#!/bin/ash
set -ex

apk update

apk del --purge build-base cmake ninja pkgconfig boost-dev libressl-dev protobuf-dev qt5-qtbase-dev
