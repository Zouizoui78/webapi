#ifndef RESULT_HPP
#define RESULT_HPP

#include <optional>
#include <string>

namespace webapi {

enum ResultCodes { OK, SQL_ERROR, ID_NOT_FOUND };

namespace messages {

static const std::string SQL_ERROR("Internal database error");
static const std::string ID_NOT_FOUND("ID not found in database");

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