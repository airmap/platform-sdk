#!/bin/sh
set -e

# This file demonstrates how to use the airmap CLI and some common UNIX
# text processing tools to reuse the output produced by the airmap CLI.
# Please note that all logging output goes to stderr, all relevant output goes to stdout.

echo "• logging in ...........\c"; login_id=$(airmap login 2> /dev/null | head -n 2 | tail -n 1 | cut -d ' ' -f 2); echo " ✔"
echo "• querying pilot id ....\c"; pilot_id=$(airmap pilot 2> /dev/null | head -n 2 | tail -n 1 | cut -d ' ' -f 1); echo " ✔"
echo "• querying aircraft id .\c"; first_aircraft_id=$(airmap pilot 2> /dev/null | head -n 4 | tail -n 1 | cut -d ' ' -f 1); echo " ✔"
echo "• creating flight ......\c"; flight_id=$(airmap create-flight --aircraft-id=${first_aircraft_id} --latitude=52.522677 --longitude=13.386201 --buffer=100 2> /dev/null | head -n 2 | tail -n 1 | cut -d ' ' -f 1); echo " ✔"
echo "• ending flight ........\c"; flight_ended=$(airmap end-flight --flight-id=${flight_id} 2> /dev/null | head -n 2 | tail -n 1); echo " ✔"
echo "• login id:             $(echo ${login_id} | cut -c 1-32)…"
echo "• pilot id:             ${pilot_id}"
echo "• first aircraft id:    ${first_aircraft_id}"
echo "• flight id:            ${flight_id}"
echo "• flight ended:         ${flight_ended}"
