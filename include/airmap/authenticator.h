#ifndef AIRMAP_AUTHENTICATOR_H_
#define AIRMAP_AUTHENTICATOR_H_

#include <airmap/credentials.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/outcome.h>

#include <chrono>
#include <functional>
#include <stdexcept>
#include <string>

namespace airmap {

class Authenticator : DoNotCopyOrMove {
 public:
  enum class Scope { access_token = 0, open_id = 1, open_id_offline_access = 2 };

  enum class GrantType { password = 0, bearer = 1 };

  enum class Connection { username_password_authentication = 0 };

  struct AnonymousToken {
    std::string id;
  };

  struct OAuthToken {
    enum class Type { bearer };
    Type type;
    std::string refresh;
    std::string id;
    std::string access;
  };

  struct RefreshedToken {
    enum class Type { bearer };
    Type type;
    std::chrono::seconds expires_in;
    std::string id;
  };

  struct AuthenticateWithPassword {
    struct Params {
      Credentials::OAuth oauth;
      GrantType grant_type{GrantType::password};
      Scope scope{Scope::open_id_offline_access};
      Connection connection{Connection::username_password_authentication};
    };

    using Result   = Outcome<OAuthToken, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct AuthenticateAnonymously {
    using Params   = Credentials::Anonymous;
    using Result   = Outcome<AnonymousToken, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct RenewAuthentication {
    struct Params {
      std::string client_id;
      std::string refresh_token;
      GrantType grant_type{GrantType::bearer};
      Scope scope{Scope::open_id};
    };
    using Result   = Outcome<RefreshedToken, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  /// authenticate_with_password authenticates the user described in 'params' with
  /// the AirMap services and reports the result to 'cb'.
  virtual void authenticate_with_password(const AuthenticateWithPassword::Params& params,
                                          const AuthenticateWithPassword::Callback& cb) = 0;

  // authenticate_anonymously authenticates an anonymous user described by Params::user_id
  // with the AirMap services and reports the result to 'cb'.
  virtual void authenticate_anonymously(const AuthenticateAnonymously::Params&,
                                        const AuthenticateAnonymously::Callback&) = 0;

  /// renew_authentication renews a pre-authenticated JWT as given in Params::user_id with
  /// the AirMap services and reports the result to 'cb'.
  virtual void renew_authentication(const RenewAuthentication::Params& params,
                                    const RenewAuthentication::Callback& cb) = 0;

 protected:
  Authenticator() = default;
};

}  // namespace airmap

#endif  // AIRMAP_AUTHENTICATOR_H_
