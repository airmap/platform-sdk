# Hacking

This document summarizes guidelines and helpful options when hacking on the `airmapd` codebase.

## Selectively Redirecting APIs to Custom Host and Port

Occasionally, during local development, being able to selectively redirect requests to specific AirMap APIs is helpful.
For that, the following environment variables can be set:

 * `AIRMAP_HOST_AIRCRAFTS` and `AIRMAP_PORT_AIRCRAFTS` for API `aircrafts`
 * `AIRMAP_HOST_AIRSPACES` and `AIRMAP_PORT_AIRSPACES` for API `airspaces`
 * `AIRMAP_HOST_AUTHENTICATOR` and `AIRMAP_PORT_AUTHENTICATOR` for authentication purposes
 * `AIRMAP_HOST_FLIGHTS` and `AIRMAP_PORT_FLIGHTS` for API `flights`
 * `AIRMAP_HOST_PILOTS` and `AIRMAP_PORT_PILOTS` for API `pilots`
 * `AIRMAP_HOST_STATUS` and `AIRMAP_PORT_STATUS` for API `status`
 * `AIRMAP_HOST_SSO` and `AIRMAP_PORT_SSO` for the SSO endpoint

An example invocation of `airmap` redirecting API flights to `localhost:3000` would look like:
```
AIRMAP_HOST_FLIGHTS=localhost AIRMAP_PORT_FLIGHTS=3000 airmap test --test-suite=laanc.phoenix.zoo
```
