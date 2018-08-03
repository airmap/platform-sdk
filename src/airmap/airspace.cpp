#include <airmap/airspace.h>

#include <iostream>

namespace {

struct Comma {
  bool first = true;
};

std::ostream &operator<<(std::ostream &out, Comma &comma) {
  if (not comma.first)
    out << ",";
  comma.first = false;
  return out;
}

}  // namespace

airmap::Airspace::Airspace() : color_{"#f7de16"}, type_{Type::invalid} {
}

airmap::Airspace::Airspace(const Airspace &rhs)
    : id_{rhs.id_},
      name_{rhs.name_},
      color_{rhs.color_},
      type_{Type::invalid},
      country_{rhs.country_},
      state_{rhs.state_},
      city_{rhs.city_},
      last_updated_{rhs.last_updated_},
      geometry_{rhs.geometry_},
      related_geometries_{rhs.related_geometries_},
      rules_{rhs.rules_} {
  set_details(rhs);
}

airmap::Airspace::~Airspace() {
  reset();
}

airmap::Airspace &airmap::Airspace::operator=(const Airspace &rhs) {
  reset();

  id_                 = rhs.id_;
  name_               = rhs.name_;
  color_              = rhs.color_;
  type_               = rhs.type_;
  country_            = rhs.country_;
  state_              = rhs.state_;
  city_               = rhs.city_;
  last_updated_       = rhs.last_updated_;
  geometry_           = rhs.geometry_;
  related_geometries_ = rhs.related_geometries_;
  rules_              = rhs.rules_;

  set_details(rhs);

  return *this;
}

bool airmap::Airspace::operator==(const Airspace &rhs) const {
  auto members_equal = id() == rhs.id() && name() == rhs.name() && color() == rhs.color() &&
                       country() == rhs.country() && state() == rhs.state() && city() == rhs.city() &&
                       last_updated() == rhs.last_updated() && geometry() == rhs.geometry() &&
                       related_geometries() == rhs.related_geometries() && rules() == rhs.rules();

  if (!members_equal)
    return false;

  switch (type()) {
    case Type::invalid:
      return true;
    case Type::airport:
      return details_for_airport() == rhs.details_for_airport();
    case Type::controlled_airspace:
      return details_for_controlled_airspace() == rhs.details_for_controlled_airspace();
    case Type::special_use_airspace:
      return details_for_special_use_airspace() == rhs.details_for_special_use_airspace();
    case Type::tfr:
      return details_for_temporary_flight_restriction() == rhs.details_for_temporary_flight_restriction();
    case Type::wildfire:
      return details_for_wildfire() == rhs.details_for_wildfire();
    case Type::park:
      return details_for_park() == rhs.details_for_park();
    case Type::power_plant:
      return details_for_power_plant() == rhs.details_for_power_plant();
    case Type::heliport:
      return details_for_heliport() == rhs.details_for_heliport();
    case Type::prison:
      return details_for_prison() == rhs.details_for_prison();
    case Type::school:
      return details_for_school() == rhs.details_for_school();
    case Type::hospital:
      return details_for_hospital() == rhs.details_for_hospital();
    case Type::fire:
      return details_for_fire() == rhs.details_for_fire();
    case Type::emergency:
      return details_for_emergency() == rhs.details_for_emergency();
    default:
      break;
  }

  return false;
}

bool airmap::Airspace::operator!=(const Airspace &rhs) const {
  return !(*this == rhs);
}

const airmap::Airspace::Id &airmap::Airspace::id() const {
  return id_;
}

void airmap::Airspace::set_id(const Id &id) {
  id_ = id;
}

const std::string &airmap::Airspace::name() const {
  return name_;
}

void airmap::Airspace::set_name(const std::string &name) {
  name_ = name;
}

const std::string &airmap::Airspace::color() const {
  return color_;
}

void airmap::Airspace::set_color(const std::string &color) {
  color_ = color;
}

airmap::Airspace::Type airmap::Airspace::type() const {
  return type_;
}

const std::string &airmap::Airspace::country() const {
  return country_;
}

void airmap::Airspace::set_country(const std::string &country) {
  country_ = country;
}

const std::string &airmap::Airspace::state() const {
  return state_;
}

void airmap::Airspace::set_state(const std::string &state) {
  state_ = state;
}

const std::string &airmap::Airspace::city() const {
  return city_;
}

void airmap::Airspace::set_city(const std::string &city) {
  city_ = city;
}

const airmap::Timestamp &airmap::Airspace::last_updated() const {
  return last_updated_;
}

void airmap::Airspace::set_last_updated(const Timestamp &timestamp) {
  last_updated_ = timestamp;
}

const airmap::Geometry &airmap::Airspace::geometry() const {
  return geometry_;
}

void airmap::Airspace::set_geometry(const Geometry &geometry) {
  geometry_ = geometry;
}

const std::map<std::string, airmap::Airspace::RelatedGeometry> &airmap::Airspace::related_geometries() const {
  return related_geometries_;
}

void airmap::Airspace::set_related_geometries(const std::map<std::string, RelatedGeometry> &geometries) {
  related_geometries_ = geometries;
}

const std::vector<airmap::Rule> &airmap::Airspace::rules() const {
  return rules_;
}

void airmap::Airspace::set_rules(const std::vector<Rule> &rules) {
  rules_ = rules;
}

const airmap::Airspace::Airport &airmap::Airspace::details_for_airport() const {
  return details_.airport;
}

airmap::Airspace::Airport &airmap::Airspace::details_for_airport() {
  return details_.airport;
}

const airmap::Airspace::ControlledAirspace &airmap::Airspace::details_for_controlled_airspace() const {
  return details_.controlled_airspace;
}

airmap::Airspace::ControlledAirspace &airmap::Airspace::details_for_controlled_airspace() {
  return details_.controlled_airspace;
}

const airmap::Airspace::Emergency &airmap::Airspace::details_for_emergency() const {
  return details_.emergency;
}

airmap::Airspace::Emergency &airmap::Airspace::details_for_emergency() {
  return details_.emergency;
}

const airmap::Airspace::Fire &airmap::Airspace::details_for_fire() const {
  return details_.fire;
}

airmap::Airspace::Fire &airmap::Airspace::details_for_fire() {
  return details_.fire;
}

const airmap::Airspace::Heliport &airmap::Airspace::details_for_heliport() const {
  return details_.heliport;
}

airmap::Airspace::Heliport &airmap::Airspace::details_for_heliport() {
  return details_.heliport;
}

const airmap::Airspace::Hospital &airmap::Airspace::details_for_hospital() const {
  return details_.hospital;
}

airmap::Airspace::Hospital &airmap::Airspace::details_for_hospital() {
  return details_.hospital;
}

const airmap::Airspace::Park &airmap::Airspace::details_for_park() const {
  return details_.park;
}

airmap::Airspace::Park &airmap::Airspace::details_for_park() {
  return details_.park;
}

const airmap::Airspace::PowerPlant &airmap::Airspace::details_for_power_plant() const {
  return details_.power_plant;
}

airmap::Airspace::PowerPlant &airmap::Airspace::details_for_power_plant() {
  return details_.power_plant;
}

const airmap::Airspace::Prison &airmap::Airspace::details_for_prison() const {
  return details_.prison;
}

airmap::Airspace::Prison &airmap::Airspace::details_for_prison() {
  return details_.prison;
}

const airmap::Airspace::School &airmap::Airspace::details_for_school() const {
  return details_.school;
}

airmap::Airspace::School &airmap::Airspace::details_for_school() {
  return details_.school;
}

const airmap::Airspace::SpecialUseAirspace &airmap::Airspace::details_for_special_use_airspace() const {
  return details_.special_use_airspace;
}

airmap::Airspace::SpecialUseAirspace &airmap::Airspace::details_for_special_use_airspace() {
  return details_.special_use_airspace;
}

const airmap::Airspace::TemporaryFlightRestriction &airmap::Airspace::details_for_temporary_flight_restriction() const {
  return details_.tfr;
}

airmap::Airspace::TemporaryFlightRestriction &airmap::Airspace::details_for_temporary_flight_restriction() {
  return details_.tfr;
}

const airmap::Airspace::Wildfire &airmap::Airspace::details_for_wildfire() const {
  return details_.wildfire;
}

airmap::Airspace::Wildfire &airmap::Airspace::details_for_wildfire() {
  return details_.wildfire;
}

void airmap::Airspace::set_details(const Airspace &detail) {
  switch (detail.type_) {
    case Type::airport:
      set_details(detail.details_for_airport());
      break;
    case Type::controlled_airspace:
      set_details(detail.details_for_controlled_airspace());
      break;
    case Type::special_use_airspace:
      set_details(detail.details_for_special_use_airspace());
      break;
    case Type::tfr:
      set_details(detail.details_for_temporary_flight_restriction());
      break;
    case Type::wildfire:
      set_details(detail.details_for_wildfire());
      break;
    case Type::park:
      set_details(detail.details_for_park());
      break;
    case Type::power_plant:
      set_details(detail.details_for_power_plant());
      break;
    case Type::heliport:
      set_details(detail.details_for_heliport());
      break;
    case Type::prison:
      set_details(detail.details_for_prison());
      break;
    case Type::school:
      set_details(detail.details_for_school());
      break;
    case Type::hospital:
      set_details(detail.details_for_hospital());
      break;
    case Type::fire:
      set_details(detail.details_for_fire());
      break;
    case Type::emergency:
      set_details(detail.details_for_emergency());
      break;
    default:
      break;
  }

  type_ = detail.type_;
}

void airmap::Airspace::set_details(const Airport &detail) {
  type_  = Type::airport;
  color_ = "#f6a517";
  new (&details_.airport) Airport(detail);
}

void airmap::Airspace::set_details(const ControlledAirspace &detail) {
  type_ = Type::controlled_airspace;
  if (detail.airspace_classification == "B")
    color_ = "#1fa0d3";
  else if (detail.airspace_classification == "D")
    color_ = "#1a74b3";
  else
    color_ = "#9b6c9d";
  new (&details_.controlled_airspace) ControlledAirspace{detail};
}

void airmap::Airspace::set_details(const SpecialUseAirspace &detail) {
  type_  = Type::special_use_airspace;
  color_ = "#1b5acf";
  new (&details_.special_use_airspace) SpecialUseAirspace{detail};
}

void airmap::Airspace::set_details(const TemporaryFlightRestriction &detail) {
  type_  = Type::tfr;
  color_ = "#f44336";
  new (&details_.tfr) TemporaryFlightRestriction(detail);
}

void airmap::Airspace::set_details(const Wildfire &detail) {
  type_  = Type::wildfire;
  color_ = "#f44336";
  new (&details_.wildfire) Wildfire{detail};
}

void airmap::Airspace::set_details(const Park &detail) {
  type_  = Type::park;
  color_ = "#e01212";
  new (&details_.park) Park{detail};
}

void airmap::Airspace::set_details(const PowerPlant &detail) {
  type_  = Type::power_plant;
  color_ = "#f6a517";
  new (&details_.power_plant) PowerPlant{detail};
}

void airmap::Airspace::set_details(const Heliport &detail) {
  type_  = Type::heliport;
  color_ = "#f6a517";
  new (&details_.heliport) Heliport{detail};
}

void airmap::Airspace::set_details(const Prison &detail) {
  type_  = Type::prison;
  color_ = "#f6a517";
  new (&details_.prison) Prison{detail};
}

void airmap::Airspace::set_details(const School &detail) {
  type_  = Type::school;
  color_ = "#f6a517";
  new (&details_.school) School{detail};
}

void airmap::Airspace::set_details(const Hospital &detail) {
  type_  = Type::hospital;
  color_ = "#f6a517";
  new (&details_.hospital) Hospital{detail};
}

void airmap::Airspace::set_details(const Fire &detail) {
  type_  = Type::fire;
  color_ = "#f44336";
  new (&details_.fire) Fire{detail};
}

void airmap::Airspace::set_details(const Emergency &detail) {
  type_  = Type::emergency;
  color_ = "#f67117";
  new (&details_.emergency) Emergency{detail};
}

void airmap::Airspace::reset() {
  switch (type_) {
    case Type::airport:
      details_.airport.~Airport();
      break;
    case Type::controlled_airspace:
      details_.controlled_airspace.~ControlledAirspace();
      break;
    case Type::special_use_airspace:
      details_.special_use_airspace.~SpecialUseAirspace();
      break;
    case Type::tfr:
      details_.tfr.~TemporaryFlightRestriction();
      break;
    case Type::wildfire:
      details_.wildfire.~Wildfire();
      break;
    case Type::park:
      details_.park.~Park();
      break;
    case Type::power_plant:
      details_.power_plant.~PowerPlant();
      break;
    case Type::heliport:
      details_.heliport.~Heliport();
      break;
    case Type::prison:
      details_.prison.~Prison();
      break;
    case Type::school:
      details_.school.~School();
      break;
    case Type::hospital:
      details_.hospital.~Hospital();
      break;
    case Type::fire:
      details_.fire.~Fire();
      break;
    case Type::emergency:
      details_.emergency.~Emergency();
      break;
    default:
      break;
  }
}

airmap::Airspace::Details::Details() : invalid{} {
}
airmap::Airspace::Details::~Details() {
}

bool airmap::operator==(const Airspace::RelatedGeometry &lhs, const Airspace::RelatedGeometry &rhs) {
  return lhs.id == rhs.id && lhs.geometry == rhs.geometry;
}

bool airmap::operator==(const Airspace::Airport &lhs, const Airspace::Airport &rhs) {
  return lhs.iata == rhs.iata && lhs.icao == rhs.icao && lhs.paved == rhs.paved && lhs.phone == rhs.phone &&
         lhs.tower == rhs.tower && lhs.runways == rhs.runways && lhs.elevation == rhs.elevation &&
         lhs.longest_runway == rhs.longest_runway &&
         lhs.instrument_approach_procedure == rhs.instrument_approach_procedure && lhs.use == rhs.use;
}

bool airmap::operator==(const Airspace::Airport::Runway &lhs, const Airspace::Airport::Runway &rhs) {
  return lhs.name == rhs.name && lhs.length == rhs.length && lhs.bearing == rhs.bearing;
}

bool airmap::operator==(const Airspace::ControlledAirspace &, const Airspace::ControlledAirspace &) {
  return true;
}

bool airmap::operator==(const Airspace::SpecialUseAirspace &lhs, const Airspace::SpecialUseAirspace &rhs) {
  return lhs.type == rhs.type;
}

bool airmap::operator==(const Airspace::TemporaryFlightRestriction &lhs,
                        const Airspace::TemporaryFlightRestriction &rhs) {
  return lhs.url == rhs.url && lhs.type == rhs.type && lhs.reason == rhs.reason;
}

bool airmap::operator==(const Airspace::Wildfire &lhs, const Airspace::Wildfire &rhs) {
  return lhs.effective_date == rhs.effective_date;
}

bool airmap::operator==(const Airspace::Park &, const Airspace::Park &) {
  return true;
}

bool airmap::operator==(const Airspace::Prison &, const Airspace::Prison &) {
  return true;
}

bool airmap::operator==(const Airspace::School &, const Airspace::School &) {
  return true;
}

bool airmap::operator==(const Airspace::Hospital &, const Airspace::Hospital &) {
  return true;
}

bool airmap::operator==(const Airspace::Fire &, const Airspace::Fire &) {
  return true;
}

bool airmap::operator==(const Airspace::Emergency &, const Airspace::Emergency &) {
  return true;
}

bool airmap::operator==(const Airspace::Heliport &lhs, const Airspace::Heliport &rhs) {
  return lhs.faa_id == rhs.faa_id && lhs.phone == rhs.phone && lhs.usage == rhs.usage;
}

bool airmap::operator==(const Airspace::PowerPlant &lhs, const Airspace::PowerPlant &rhs) {
  return lhs.technology == rhs.technology && lhs.code == rhs.code;
}

airmap::Airspace::Type airmap::operator~(airmap::Airspace::Type type) {
  using UT = typename std::underlying_type<Airspace::Type>::type;
  return static_cast<Airspace::Type>(~static_cast<UT>(type));
}

airmap::Airspace::Type airmap::operator|(airmap::Airspace::Type lhs, airmap::Airspace::Type rhs) {
  using UT = typename std::underlying_type<Airspace::Type>::type;
  return static_cast<Airspace::Type>(static_cast<UT>(lhs) | static_cast<UT>(rhs));
}

airmap::Airspace::Type airmap::operator&(airmap::Airspace::Type lhs, airmap::Airspace::Type rhs) {
  using UT = typename std::underlying_type<Airspace::Type>::type;
  return static_cast<Airspace::Type>(static_cast<UT>(lhs) & static_cast<UT>(rhs));
}

std::ostream &airmap::operator<<(std::ostream &out, const Airspace &) {
  return out;
}

std::ostream &airmap::operator<<(std::ostream &out, Airspace::Type types) {
  if (types == airmap::Airspace::Type::invalid)
    return out;

  Comma comma;

  if ((types & airmap::Airspace::Type::airport) == airmap::Airspace::Type::airport)
    out << comma << "airport";
  if ((types & airmap::Airspace::Type::controlled_airspace) == airmap::Airspace::Type::controlled_airspace)
    out << comma << "controlled_airspace";
  if ((types & airmap::Airspace::Type::special_use_airspace) == airmap::Airspace::Type::special_use_airspace)
    out << comma << "special_use_airspace";
  if ((types & airmap::Airspace::Type::tfr) == airmap::Airspace::Type::tfr)
    out << comma << "tfr";
  if ((types & airmap::Airspace::Type::wildfire) == airmap::Airspace::Type::wildfire)
    out << comma << "wildfire";
  if ((types & airmap::Airspace::Type::park) == airmap::Airspace::Type::park)
    out << comma << "park";
  if ((types & airmap::Airspace::Type::power_plant) == airmap::Airspace::Type::power_plant)
    out << comma << "power_plant";
  if ((types & airmap::Airspace::Type::heliport) == airmap::Airspace::Type::heliport)
    out << comma << "heliport";
  if ((types & airmap::Airspace::Type::prison) == airmap::Airspace::Type::prison)
    out << comma << "prison";
  if ((types & airmap::Airspace::Type::school) == airmap::Airspace::Type::school)
    out << comma << "school";
  if ((types & airmap::Airspace::Type::hospital) == airmap::Airspace::Type::hospital)
    out << comma << "hospital";
  if ((types & airmap::Airspace::Type::fire) == airmap::Airspace::Type::fire)
    out << comma << "fire";
  if ((types & airmap::Airspace::Type::emergency) == airmap::Airspace::Type::emergency)
    out << comma << "emergency";

  return out;
}
