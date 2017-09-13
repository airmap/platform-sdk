#ifndef AIRMAP_FLIGHTS_H_
#define AIRMAP_FLIGHTS_H_

#include <airmap/date_time.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/flight.h>
#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/outcome.h>

#include <cstdint>
#include <functional>
#include <vector>

namespace airmap {

class Flights : DoNotCopyOrMove {
 public:
  struct ForId {
    struct Parameters {
      Optional<std::string> authorization;
      Flight::Id id;
      Optional<bool> enhance;
    };

    using Result   = Outcome<Flight, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct Search {
    struct Parameters {
      Optional<std::string> authorization;
      Optional<std::uint32_t> limit;
      Optional<Geometry> geometry;
      Optional<std::string> country;
      Optional<std::string> state;
      Optional<std::string> city;
      Optional<std::string> pilot_id;
      Optional<DateTime> start_after;
      Optional<DateTime> start_before;
      Optional<DateTime> end_after;
      Optional<DateTime> end_before;
      Optional<bool> enhance;
    };
    using Result   = Outcome<std::vector<Flight>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct CreateFlight {
    struct Parameters {
      std::string authorization;
      Required<float> latitude;
      Required<float> longitude;
      float max_altitude = 121.;
      std::string aircraft_id;
      DateTime start_time;
      DateTime end_time;
      bool is_public           = true;
      bool give_digital_notice = true;
      float buffer             = 100;
      Optional<Geometry> geometry;
    };
    using Result   = Outcome<Flight, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct DeleteFlight {
    struct Parameters {
      std::string authorization;
      Flight::Id id;
    };

    struct Response {
      Flight::Id id;
    };

    using Result   = Outcome<Response, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct EndFlight {
    struct Parameters {
      std::string authorization;
      Flight::Id id;
    };

    struct Response {
      DateTime end_time;
    };

    using Result   = Outcome<Response, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct StartFlightCommunications {
    struct Parameters {
      std::string authorization;
      Flight::Id id;
    };

    struct Response {
      std::string key;
    };

    using Result   = Outcome<Response, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct EndFlightCommunications {
    struct Parameters {
      std::string authorization;
      Flight::Id id;
    };

    struct Response {};

    using Result   = Outcome<Response, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  /// search queries the AirMap services for known flights
  /// and reports results to 'cb'.
  virtual void search(const Search::Parameters& parameters, const Search::Callback& cb) = 0;

  /// for_ids queries the AirMap services for detailed information about
  /// flights identified by UUIDs and reports back results to 'cb'.
  virtual void for_id(const ForId::Parameters& parameters, const ForId::Callback& cb) = 0;

  /// create_flight creates a flight for 'parameters' and reports
  /// results back to 'cb'.
  virtual void create_flight_by_point(const CreateFlight::Parameters& parameters, const CreateFlight::Callback& cb) = 0;

  /// create_flight creates a flight for 'parameters' and reports
  /// results back to 'cb'.
  virtual void create_flight_by_path(const CreateFlight::Parameters& parameters, const CreateFlight::Callback& cb) = 0;

  /// create_flight creates a flight for 'parameters' and reports
  /// results back to 'cb'.
  virtual void create_flight_by_polygon(const CreateFlight::Parameters& parameters,
                                        const CreateFlight::Callback& cb) = 0;

  /// end_flight finalizes a flight identified by 'parameters' and reports
  /// results back to 'cb'.
  virtual void end_flight(const EndFlight::Parameters& parameters, const EndFlight::Callback& cb) = 0;

  /// delete_flight deletes a flight identified by 'parameters' and reports
  /// results back to 'cb'.
  virtual void delete_flight(const DeleteFlight::Parameters& parameters, const DeleteFlight::Callback& cb) = 0;

  /// start_flight_communications enables communications for a specific flight
  /// instance and reports results back to 'cb'.
  virtual void start_flight_communications(const StartFlightCommunications::Parameters& parameters,
                                           const StartFlightCommunications::Callback& cb) = 0;

  /// end_flight_communications enables communications for a specific flight
  /// instance and reports results back to cb.
  virtual void end_flight_communications(const EndFlightCommunications::Parameters& parameters,
                                         const EndFlightCommunications::Callback& cb) = 0;

 protected:
  Flights() = default;
};

}  // namespace airmap

#endif  // AIRMAP_AIRSPACES_H_
