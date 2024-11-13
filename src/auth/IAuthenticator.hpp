#ifndef AUTHENTICATOR_HPP
#define AUTHENTICATOR_HPP

#include <string>

#include "Result.hpp"

namespace webapi::auth {

// Interface for objects in charge of authentication.
// The token is expected to be the content of the Authorization header.
class IAuthenticator {
public:
  virtual Result<> authenticate(std::string_view token,
                                std::string_view route) const = 0;

  IAuthenticator() = default;
  virtual ~IAuthenticator() = default;
  IAuthenticator(const IAuthenticator &other) = delete;
  IAuthenticator(IAuthenticator &&other) = delete;
  IAuthenticator &operator=(const IAuthenticator &other) = delete;
  IAuthenticator &operator=(IAuthenticator &&other) = delete;
};

} // namespace webapi::auth

#endif