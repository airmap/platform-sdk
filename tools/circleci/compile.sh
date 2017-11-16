#!/bin/sh

set -e

test_pilot_099_api_key="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJjcmVkZW50aWFsX2lkIjoiY3JlZGVudGlhbHxrTjV4bERwaWdKQmtnbGlsR3BxYUhtR1Fxa20iLCJhcHBsaWNhdGlvbl9pZCI6ImFwcGxpY2F0aW9ufDVLcTVrODJJS2s1YTdPU3lYTjNhSmluUU15OSIsIm9yZ2FuaXphdGlvbl9pZCI6ImRldmVsb3Blcnx4RTRiRWw4dVhvV3ZKbWY2eTdYN29IRU5Ld1BEIiwiaWF0IjoxNTA4ODg0MDkxfQ.6diSTMGUELDMS-HUJNXp3HqRQ8L7CQq7glaw3Z0C59o"
test_pilot_099_client_id="yPbA5PDvD0zRiXcFqIlu4eDKdRGgKGGH"
test_pilot_099_device_id="alpine"
test_pilot_099_username="TestPilot099@airmap.com"
test_pilot_099_password="TestPilotsAreKool099"

test_pilot_098_api_key="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJjcmVkZW50aWFsX2lkIjoiY3JlZGVudGlhbHx6MzN6T0dEY21wbjV3NmZHTVdwUFpJblBEd1puIiwiYXBwbGljYXRpb25faWQiOiJhcHBsaWNhdGlvbnxwNE4ySnF2aGc3cFpxWVM2dzZ2eFh0Z2czeDV4Iiwib3JnYW5pemF0aW9uX2lkIjoiZGV2ZWxvcGVyfEt5cURrM0p0N2VuUGJLVVk0QU5iNUllMEFXbkQiLCJpYXQiOjE1MDg4ODU1NTN9.K3ejcgnoyip3u59ba-VBCivs6tn5gahOsI9FYkCI464"
test_pilot_098_client_id="fW6JvVE63pPuru6Og7h37Yc7667j96B1"
test_pilot_098_device_id="ubuntu"
test_pilot_098_username="TestPilot098@airmap.com"
test_pilot_098_password="TestPilotsAreKool098"

docker build \
    --build-arg CIRCLECI=true \
    --build-arg AIRMAP_CIRCLECI_API_KEY=${test_pilot_099_api_key} \
    --build-arg AIRMAP_CIRCLECI_CLIENT_ID=${test_pilot_099_client_id} \
    --build-arg AIRMAP_CIRCLECI_DEVICE_ID=${test_pilot_099_device_id} \
    --build-arg AIRMAP_CIRCLECI_USERNAME=${test_pilot_099_username} \
    --build-arg AIRMAP_CIRCLECI_PASSWORD=${test_pilot_099_password} --force-rm=true -t airmapd.alpine:latest -f docker/alpine .

docker build \
    --build-arg CIRCLECI=true \
    --build-arg AIRMAP_CIRCLECI_API_KEY=${test_pilot_098_api_key} \
    --build-arg AIRMAP_CIRCLECI_CLIENT_ID=${test_pilot_098_client_id} \
    --build-arg AIRMAP_CIRCLECI_DEVICE_ID=${test_pilot_098_device_id} \
    --build-arg AIRMAP_CIRCLECI_USERNAME=${test_pilot_098_username} \
    --build-arg AIRMAP_CIRCLECI_PASSWORD=${test_pilot_098_password} --force-rm=true -t airmapd.ubuntu:latest -f docker/ubuntu .

docker run             \
    -v $(pwd):/airmapd \
    -w /airmapd        \
    ubuntu:16.04       \
    /airmapd/tools/ubuntu/build-package.sh
