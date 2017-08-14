#ifndef AIRMAP_CLIENT_H_
#define AIRMAP_CLIENT_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/outcome.h>

#include <functional>
#include <memory>
#include <string>

namespace airmap {

class Aircrafts;
class Airspaces;
class Authenticator;
class Flights;
class Telemetry;
class Traffic;

/// Client enables applications to use the AirMap services and APIs.
class Client : DoNotCopyOrMove {
 public:
  /// Credentials bundles up all credentials required
  /// to use the AirMap SDK and APIs.
  struct Credentials {
    std::string api_key;
  };

  /// authenticator returns the Authenticator implementation provided by the client.
  virtual Authenticator& authenticator() = 0;

  /// aircrafts returns the Aircrafts implementation provided by the client.
  virtual Aircrafts& aircrafts() = 0;

  /// airspaces returns the Airspaces implementation provided by the client.
  virtual Airspaces& airspaces() = 0;

  /// flights returns the Flights implementation provided by the client.
  virtual Flights& flights() = 0;

  /// telemetry returns the Telemetry implementation provided by the client.
  virtual Telemetry& telemetry() = 0;

  /// traffic returns the Traffic implementation provided by the client.
  virtual Traffic& traffic() = 0;

 protected:
  Client() = default;
};

}  // namespace airmap

#endif  // AIRMAP_CLIENT_H_
