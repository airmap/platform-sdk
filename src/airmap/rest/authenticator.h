#ifndef AIRMAP_REST_AUTHENTICATOR_H_
#define AIRMAP_REST_AUTHENTICATOR_H_

#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/net/http/requester.h>

namespace airmap {
namespace rest {

class Authenticator : public airmap::Authenticator {
 public:
  explicit Authenticator(Client::Version version, const std::shared_ptr<net::http::Requester>& airmap_requester,
                         const std::shared_ptr<net::http::Requester>& sso_requester);

  void authenticate_with_password(const AuthenticateWithPassword::Params& params,
                                  const AuthenticateWithPassword::Callback& cb) override;

  void authenticate_anonymously(const AuthenticateAnonymously::Params& params,
                                const AuthenticateAnonymously::Callback& cb) override;

  void renew_authentication(const RenewAuthentication::Params& params,
                            const RenewAuthentication::Callback& cb) override;

 private:
  Client::Version version_;
  std::shared_ptr<net::http::Requester> airmap_requester_;
  std::shared_ptr<net::http::Requester> sso_requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_AUTHENTICATOR_H_
