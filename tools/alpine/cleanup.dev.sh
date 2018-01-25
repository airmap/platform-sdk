#!/bin/ash
set -ex

apk update

apk del --purge build-base cmake git ninja pkgconfig libressl-dev qt5-qtbase-dev
