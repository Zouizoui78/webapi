#include "BasicAuthenticator.hpp"

#include "base64.h"

namespace webapi::auth {

using namespace services;

BasicAuthenticator::BasicAuthenticator(UserService *service)
    : _service(service) {}

Result<> BasicAuthenticator::authenticate(std::string_view token,
                                          std::string_view route) const {
  if (!token.starts_with("Basic")) {
    return Result<>{.code = AUTH_INVALID_TOKEN,
                    .message = messages::AUTH_INVALID_TOKEN};
  }

  if (!route.starts_with("/api/login")) {
    return Result<>{.code = AUTH_BASIC_LOGIN_ONLY,
                    .message = messages::AUTH_BASIC_LOGIN_ONLY};
  }

  constexpr std::size_t token_start = std::strlen("Basic ");
  token = token.substr(token_start);
  std::string decoded_token = base64_decode(token, false);

  auto colon = decoded_token.find(":");
  if (colon == std::string::npos) {
    return Result<>{.code = AUTH_INVALID_TOKEN,
                    .message = messages::AUTH_INVALID_TOKEN};
  }

  std::string username = decoded_token.substr(0, colon);
  std::string password = decoded_token.substr(colon + 1);
  std::size_t hash = std::hash<std::string>{}(password);
  std::string hash_str = std::to_string(hash);

  auto res = _service->get_user_password(username);
  if (res.code != OK) {
    return Result<>{.code = res.code, .message = res.message};
  }

  if (hash_str != res.value) {
    return Result<>{.code = AUTH_WRONG_PASSWORD,
                    .message = messages::AUTH_WRONG_PASSWORD};
  }

  return Result<>();
}

} // namespace webapi::auth