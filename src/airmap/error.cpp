#include <airmap/error.h>

#include <cassert>

namespace {

bool is_basic_type(const airmap::Error::Value& value) {
  switch (value.type()) {
    case airmap::Error::Value::Type::dictionary:
    case airmap::Error::Value::Type::vector:
      return false;
    default:
      return true;
  }

  return false;
}

}  // namespace

airmap::Error::Value::Value() : type_{Type::undefined} {
}

airmap::Error::Value::Value(bool value) : type_{Type::undefined} {
  construct(value);
}

airmap::Error::Value::Value(std::int64_t value) : type_{Type::undefined} {
  construct(value);
}

airmap::Error::Value::Value(double value) : type_{Type::undefined} {
  construct(value);
}

airmap::Error::Value::Value(const std::string& value) : type_{Type::undefined} {
  construct(value);
}

airmap::Error::Value::Value(const std::vector<std::uint8_t>& value) : type_{Type::undefined} {
  construct(value);
}

airmap::Error::Value::Value(const std::map<Value, Value>& value) : type_{Type::undefined} {
  construct(value);
}

airmap::Error::Value::Value(const std::vector<Value>& value) : type_{Type::undefined} {
  construct(value);
}

airmap::Error::Value::Value(const Value& value) : type_{Type::undefined} {
  construct(value);
}

airmap::Error::Value::Value(Value&& value) : type_{Type::undefined} {
  construct(value);
}

airmap::Error::Value::~Value() {
  destruct();
}

airmap::Error::Value& airmap::Error::Value::operator=(const Value& value) {
  return destruct().construct(value);
}

airmap::Error::Value& airmap::Error::Value::operator=(Value&& value) {
  return destruct().construct(value);
}

airmap::Error::Value::Type airmap::Error::Value::type() const {
  return type_;
}

bool airmap::Error::Value::boolean() const {
  return details_.boolean;
}

std::int64_t airmap::Error::Value::integer() const {
  return details_.integer;
}

double airmap::Error::Value::floating_point() const {
  return details_.floating_point;
}

const std::string& airmap::Error::Value::string() const {
  return details_.string;
}

const std::vector<std::uint8_t> airmap::Error::Value::blob() const {
  return details_.blob;
}

const std::map<airmap::Error::Value, airmap::Error::Value>& airmap::Error::Value::dictionary() const {
  return details_.dictionary;
}

const std::vector<airmap::Error::Value>& airmap::Error::Value::vector() const {
  return details_.vector;
}

airmap::Error::Value& airmap::Error::Value::construct(bool value) {
  new (&details_.boolean) bool(value);
  type_ = Type::boolean;

  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(std::int64_t value) {
  new (&details_.integer) std::int64_t(value);
  type_ = Type::integer;

  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(double value) {
  new (&details_.floating_point) double(value);
  type_ = Type::floating_point;

  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(const std::string& value) {
  new (&details_.string) std::string(value);
  type_ = Type::string;

  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(std::string&& value) {
  new (&details_.string) std::string(value);
  type_ = Type::string;

  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(const std::vector<std::uint8_t>& value) {
  new (&details_.blob) std::vector<std::uint8_t>(value);
  type_ = Type::blob;

  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(std::vector<std::uint8_t>&& value) {
  new (&details_.blob) std::vector<std::uint8_t>(value);
  type_ = Type::blob;

  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(const std::map<Value, Value>& value) {
  new (&details_.dictionary) std::map<Value, Value>(value);
  type_ = Type::dictionary;

  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(std::map<Value, Value>&& value) {
  new (&details_.dictionary) std::map<Value, Value>(value);
  type_ = Type::dictionary;

  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(const std::vector<Value>& value) {
  new (&details_.vector) std::vector<Value>(value);
  type_ = Type::vector;

  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(std::vector<Value>&& value) {
  new (&details_.vector) std::vector<Value>(value);
  type_ = Type::vector;

  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(const Value& value) {
  switch (value.type()) {
    case Type::boolean:
      return construct(value.boolean());
    case Type::integer:
      return construct(value.integer());
    case Type::floating_point:
      return construct(value.floating_point());
    case Type::string:
      return construct(value.string());
    case Type::blob:
      return construct(value.blob());
    case Type::dictionary:
      return construct(value.dictionary());
    case Type::vector:
      return construct(value.vector());
    default:
      break;
  }
  assert("should not reach here");
  return *this;
}

airmap::Error::Value& airmap::Error::Value::construct(Value&& value) {
  switch (value.type()) {
    case Type::boolean:
      return construct(value.boolean());
    case Type::integer:
      return construct(value.integer());
    case Type::floating_point:
      return construct(value.floating_point());
    case Type::string:
      return construct(value.string());
    case Type::blob:
      return construct(value.blob());
    case Type::dictionary:
      return construct(value.dictionary());
    case Type::vector:
      return construct(value.vector());
    default:
      break;
  }
  assert("should not reach here");
  return *this;
}

airmap::Error::Value& airmap::Error::Value::destruct() {
  type_ = Type::undefined;

  switch (type_) {
    case Type::undefined:
    case Type::boolean:
    case Type::integer:
    case Type::floating_point:
      break;
    case Type::string:
      using String = std::string;
      details_.string.~String();
    case Type::blob:
      using Blob = std::vector<std::uint8_t>;
      details_.blob.~Blob();
    case Type::dictionary:
      using Dictionary = std::map<Value, Value>;
      details_.dictionary.~Dictionary();
    case Type::vector:
      using Vector = std::vector<Value>;
      details_.vector.~Vector();
    default:
      break;
  }

  return *this;
}

airmap::Error::Value::Details::Details() {
}

airmap::Error::Value::Details::~Details() {
}

bool airmap::operator==(const Error::Value& lhs, const Error::Value& rhs) {
  if (lhs.type() != rhs.type()) {
    return false;
  }

  switch (lhs.type()) {
    case Error::Value::Type::boolean:
      return lhs.boolean() == rhs.boolean();
    case Error::Value::Type::integer:
      return lhs.integer() == rhs.integer();
    case Error::Value::Type::floating_point:
      return lhs.floating_point() == rhs.floating_point();
    case Error::Value::Type::string:
      return lhs.string() == rhs.string();
    case Error::Value::Type::blob:
      return lhs.blob() == rhs.blob();
    case Error::Value::Type::dictionary:
      return lhs.dictionary() == rhs.dictionary();
    case Error::Value::Type::vector:
      return lhs.vector() == rhs.vector();
    default:
      break;
  }

  return false;
}

bool airmap::operator<(const Error::Value& lhs, const Error::Value& rhs) {
  if (!is_basic_type(lhs) || !is_basic_type(rhs)) {
    throw std::logic_error{"cannot compare non-basic types"};
  }

  if (lhs.type() != rhs.type()) {
    using UT = typename std::underlying_type<Error::Value::Type>::type;
    return static_cast<UT>(lhs.type()) < static_cast<UT>(rhs.type());
  }

  switch (lhs.type()) {
    case Error::Value::Type::boolean:
      return lhs.boolean() < rhs.boolean();
    case Error::Value::Type::integer:
      return lhs.integer() < rhs.integer();
    case Error::Value::Type::floating_point:
      return lhs.floating_point() < rhs.floating_point();
    case Error::Value::Type::string:
      return lhs.string() < rhs.string();
    case Error::Value::Type::blob:
      return lhs.blob() < rhs.blob();
    default:
      break;
  }

  return false;
}

std::ostream& airmap::operator<<(std::ostream& out, const Error& error) {
  return out << error.message;
}