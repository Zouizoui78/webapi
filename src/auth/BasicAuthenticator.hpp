#ifndef BASIC_AUTHENTICATOR_HPP
#define BASIC_AUTHENTICATOR_HPP

#include "IAuthenticator.hpp"
#include "services/UserService.hpp"

namespace webapi::auth {

class BasicAuthenticator final : IAuthenticator {
public:
  BasicAuthenticator(services::UserService *service);

  Result<> authenticate(std::string_view token,
                        std::string_view route) const override;

private:
  services::UserService *_service = nullptr;
};

} // namespace webapi::auth

#endif