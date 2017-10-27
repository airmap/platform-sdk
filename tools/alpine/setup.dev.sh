#!/bin/ash

set -ex

apk update

apk add libressl2.5-libssl protobuf boost-system boost-program_options boost-date_time boost-filesystem
apk add build-base cmake ninja pkgconfig boost-dev libressl-dev protobuf-dev qt5-qtbase-dev
