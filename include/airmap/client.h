#ifndef AIRMAP_CLIENT_H_
#define AIRMAP_CLIENT_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/outcome.h>

#include <cstdint>

#include <functional>
#include <iosfwd>
#include <memory>
#include <string>

namespace airmap {

class Aircrafts;
class Airspaces;
class Authenticator;
class Flights;
class Pilots;
class Status;
class Telemetry;
class Traffic;

/// Client enables applications to use the AirMap services and APIs.
class Client : DoNotCopyOrMove {
 public:
  /// Version enumerates all known versions available to clients.
  enum class Version { production, staging };

  /// Credentials bundles up all credentials required
  /// to use the AirMap SDK and APIs.
  struct Credentials {
    std::string api_key;
  };

  /// Configuration bundles up parameters enabling
  /// customization of a Client implementation behavior.
  struct Configuration {
    std::string host;  ///< Address of the host exposing the AirMap services.
    Version version;   ///< The version of the AirMap services that should be used.
    struct {
      std::string host;    ///< Address of the host exposing the AirMap telemetry endpoints.
      std::uint16_t port;  ///< Port of the host exposing the AirMap telemetry endpoints.
    } telemetry;
    struct {
      std::string host;    ///< Address of the mqtt broker serving air traffic information.
      std::uint16_t port;  ///< Port of the mqtt broker serving air traffic information.
    } traffic;
    Credentials credentials;  ///< Credentials that are required to authorize access to the AirMap services.
  };

  /// default_production_configuration returns a Configuration instance that works
  /// against the AirMap production API and telemetry endpoints.
  static Configuration default_production_configuration(const Credentials& credentials);

  /// default_staging_configuration returns a Configuration instance that works
  /// against the AirMap staging API and telemetry endpoints.
  static Configuration default_staging_configuration(const Credentials& credentials);

  /// default_configuration returns a Configuration instance that works against
  /// the AirMap API and telemetry endpoints indicated by 'version'.
  static Configuration default_configuration(Version version, const Credentials& credentials);

  /// authenticator returns the Authenticator implementation provided by the client.
  virtual Authenticator& authenticator() = 0;

  /// aircrafts returns the Aircrafts implementation provided by the client.
  virtual Aircrafts& aircrafts() = 0;

  /// airspaces returns the Airspaces implementation provided by the client.
  virtual Airspaces& airspaces() = 0;

  /// flights returns the Flights implementation provided by the client.
  virtual Flights& flights() = 0;

  /// pilots returns the Pilots implementation provided by the client.
  virtual Pilots& pilots() = 0;

  /// status returns the Status implementation provided by the client.
  virtual Status& status() = 0;

  /// telemetry returns the Telemetry implementation provided by the client.
  virtual Telemetry& telemetry() = 0;

  /// traffic returns the Traffic implementation provided by the client.
  virtual Traffic& traffic() = 0;

 protected:
  Client() = default;
};

std::istream& operator>>(std::istream& in, Client::Version& version);
std::ostream& operator<<(std::ostream& out, Client::Version version);

}  // namespace airmap

#endif  // AIRMAP_CLIENT_H_
