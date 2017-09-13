#ifndef AIRMAP_TOKEN_H_
#define AIRMAP_TOKEN_H_

#include <chrono>
#include <iosfwd>
#include <string>

namespace airmap {

/// Token models an authentication token required to access the AirMap services.
class Token {
 public:
  /// Type enumerates all known token types.
  enum class Type { unknown, anonymous, oauth, refreshed };

  /// Anonymous models a token for an anonymous authentication with the AirMap services.
  struct Anonymous {
    std::string id;
  };

  /// OAuth models a token for an authentication with OAuth credentials with the AirMap services.
  struct OAuth {
    enum class Type { bearer };
    Type type;
    std::string refresh;
    std::string id;
    std::string access;
  };

  /// Refreshed models a t ocken for a refreshed authentication with OAuth credentials with the AirMap services.
  struct Refreshed {
    enum class Type { bearer };
    Type type;
    std::chrono::seconds expires_in;
    std::string id;
  };

  /// load_from_json reads a Token instance from the input stream 'in'.
  static Token load_from_json(std::istream& in);

  /// Token constructs a new invalid instance.
  explicit Token();
  /// Token constructs a new instance with Type::anonymous.
  explicit Token(const Anonymous& anonymous);
  /// Token constructs a new instance with Type::oauth.
  explicit Token(const OAuth& oauth);
  /// Token constructs a new instance with Type::refreshed.
  explicit Token(const Refreshed& refreshed);
  /// @cond
  Token(const Token& token);
  Token(Token&& token);
  ~Token();
  Token& operator=(const Token& token);
  Token& operator=(Token&& token);
  /// @endcond

  /// type returns the Type of this Token instance.
  Type type() const;
  /// id returns the common id of this Token instance.
  const std::string& id() const;
  /// anonymous returns the details for a Type::anonymous Token instance.
  const Anonymous& anonymous() const;
  /// oauth returns the details for a Type::oauth Token instance.
  const OAuth& oauth() const;
  /// refreshed returns the details for a Type::refreshed Token instance.
  const Refreshed& refreshed() const;

 private:
  union Data {
    Data();
    ~Data();

    Anonymous anonymous;
    OAuth oauth;
    Refreshed refreshed;
  };

  Token& construct(const Token& token);
  Token& construct(const Anonymous& anonymous);
  Token& construct(const OAuth& oauth);
  Token& construct(const Refreshed& refreshed);

  Token& destruct();

  Type type_;
  Data data_;
};

/// operator<< inserts type into out.
std::ostream& operator<<(std::ostream& out, Token::Type type);
/// operator>> extracts type from in.
std::istream& operator>>(std::istream& in, Token::Type& type);
/// operator== returns true iff lhs equals rhs.
bool operator==(const Token::OAuth& lhs, const Token::OAuth& rhs);
/// operator== returns true iff lhs equals rhs.
bool operator==(Token::OAuth::Type lhs, Token::OAuth::Type rhs);
/// operator== returns true iff lhs equals rhs.
bool operator==(const Token::Refreshed& lhs, const Token::Refreshed& rhs);
/// operator== returns true iff lhs equals rhs.
bool operator==(Token::Refreshed::Type lhs, Token::Refreshed::Type rhs);

}  // namespace airmap

#endif  // AIRMAP_TOKEN_H_
