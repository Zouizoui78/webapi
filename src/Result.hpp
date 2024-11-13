#ifndef RESULT_HPP
#define RESULT_HPP

#include <optional>
#include <string>

namespace webapi {

enum ResultCodes {
  OK,
  SQL_ERROR,
  ID_NOT_FOUND,
  AUTH_INVALID_TOKEN,
  AUTH_BASIC_LOGIN_ONLY,
  AUTH_WRONG_PASSWORD,
  USER_NOT_FOUND,
  PASSWORD_VALIDATION_FAILED
};

namespace messages {

static const std::string SQL_ERROR("Internal database error");
static const std::string ID_NOT_FOUND("ID not found");
static const std::string
    AUTH_INVALID_TOKEN("The provided authentication token is invalid");
static const std::string
    AUTH_BASIC_LOGIN_ONLY("The basic authentication only be used for login");
static const std::string AUTH_WRONG_PASSWORD("Wrong password");
static const std::string USER_NOT_FOUND("User not found");
static const std::string PASSWORD_VALIDATION_FAILED("Password is not valid");

} // namespace messages

// Struct holding an optional value of type T
// and an result code + message.
template <typename T = void> struct Result {
  std::optional<T> value;
  ResultCodes code = OK;
  std::string message;
};

template <> struct Result<void> {
  ResultCodes code = OK;
  std::string message;
};

} // namespace webapi

#endif