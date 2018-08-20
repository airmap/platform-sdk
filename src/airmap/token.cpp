#include <airmap/token.h>

#include <airmap/codec.h>

#include <iostream>
#include <stdexcept>

#include <jwt/jwt.hpp>

airmap::Token airmap::Token::load_from_json(std::istream& in) {
  Token result = nlohmann::json::parse(in);
  return result;
}

airmap::Token::Token() : type_{Type::unknown} {
}

airmap::Token::Token(const Anonymous& anonymous) : type_{Type::unknown} {
  construct(anonymous);
}

airmap::Token::Token(const OAuth& oauth) : type_{Type::unknown} {
  construct(oauth);
}

airmap::Token::Token(const Refreshed& refreshed) : type_{Type::unknown} {
  construct(refreshed);
}

airmap::Token::Token(const Token& token) : type_{Type::unknown} {
  construct(token);
}

airmap::Token::Token(Token&& token) : type_{Type::unknown} {
  construct(token);
}

airmap::Token::~Token() {
  destruct();
}

airmap::Token& airmap::Token::operator=(const Token& token) {
  return destruct().construct(token);
}

airmap::Token& airmap::Token::operator=(Token&& token) {
  return destruct().construct(token);
}

airmap::Token::Type airmap::Token::type() const {
  return type_;
}

const std::string airmap::Token::pilot_id() const {
  auto dec_obj = jwt::decode(id(), jwt::params::algorithms({"hs256"}), jwt::params::verify(false));
  std::ostringstream oss;
  oss << dec_obj.payload();
  auto j = nlohmann::json::parse(oss.str());
  return j["sub"];
}

const std::string& airmap::Token::id() const {
  switch (type_) {
    case Type::anonymous:
      return anonymous().id;
    case Type::oauth:
      return oauth().id;
    case Type::refreshed:
      return refreshed().id;
    default:
      break;
  }

  throw std::logic_error{"should not reach here"};
}

const airmap::Token::Anonymous& airmap::Token::anonymous() const {
  return data_.anonymous;
}

airmap::Token::Anonymous& airmap::Token::anonymous() {
  return data_.anonymous;
}

const airmap::Token::OAuth& airmap::Token::oauth() const {
  return data_.oauth;
}

airmap::Token::OAuth& airmap::Token::oauth() {
  return data_.oauth;
}

const airmap::Token::Refreshed& airmap::Token::refreshed() const {
  return data_.refreshed;
}

airmap::Token::Refreshed& airmap::Token::refreshed() {
  return data_.refreshed;
}

airmap::Token& airmap::Token::construct(const Token& token) {
  type_ = token.type_;

  switch (type_) {
    case Type::anonymous:
      new (&data_.anonymous) Anonymous(token.data_.anonymous);
      break;
    case Type::oauth:
      new (&data_.oauth) OAuth(token.data_.oauth);
      break;
    case Type::refreshed:
      new (&data_.refreshed) Refreshed(token.data_.refreshed);
      break;
    default:
      break;
  }

  return *this;
}

airmap::Token& airmap::Token::construct(const Anonymous& anonymous) {
  type_ = Type::anonymous;
  new (&data_.anonymous) Anonymous(anonymous);
  return *this;
}

airmap::Token& airmap::Token::construct(const OAuth& oauth) {
  type_ = Type::oauth;
  new (&data_.oauth) OAuth(oauth);
  return *this;
}

airmap::Token& airmap::Token::construct(const Refreshed& refreshed) {
  type_ = Type::refreshed;
  new (&data_.refreshed) Refreshed(refreshed);
  return *this;
}

airmap::Token& airmap::Token::destruct() {
  switch (type_) {
    case Type::anonymous:
      data_.anonymous.~Anonymous();
      break;
    case Type::oauth:
      data_.oauth.~OAuth();
      break;
    case Type::refreshed:
      data_.refreshed.~Refreshed();
      break;
    default:
      break;
  }

  type_ = Type::unknown;

  return *this;
}

airmap::Token::Data::Data() {
}
airmap::Token::Data::~Data() {
}

std::ostream& airmap::operator<<(std::ostream& out, Token::Type type) {
  switch (type) {
    case Token::Type::anonymous:
      out << "anonymous";
      break;
    case Token::Type::oauth:
      out << "oauth";
      break;
    case Token::Type::refreshed:
      out << "refreshed";
      break;
    case Token::Type::unknown:
      out << "unknown";
      break;
    default:
      break;
  }
  return out;
}

std::istream& airmap::operator>>(std::istream& in, Token::Type& type) {
  std::string s;
  in >> s;

  if (s == "anonymous") {
    type = Token::Type::anonymous;
  } else if (s == "oauth") {
    type = Token::Type::oauth;
  } else if (s == "refreshed") {
    type = Token::Type::refreshed;
  } else if (s == "unknown") {
    type = Token::Type::unknown;
  }

  return in;
}

bool airmap::operator==(const Token::OAuth& lhs, const Token::OAuth& rhs) {
  return lhs.type == rhs.type && lhs.refresh == rhs.refresh && lhs.id == rhs.id && lhs.access == rhs.access;
}

bool airmap::operator==(Token::OAuth::Type lhs, Token::OAuth::Type rhs) {
  using UT = typename std::underlying_type<Token::OAuth::Type>::type;
  return static_cast<UT>(lhs) == static_cast<UT>(rhs);
}

bool airmap::operator==(const Token::Refreshed& lhs, const Token::Refreshed& rhs) {
  return lhs.type == rhs.type && lhs.expires_in == rhs.expires_in && lhs.id == rhs.id;
}
