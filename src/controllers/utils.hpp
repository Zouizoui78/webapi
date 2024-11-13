#ifndef CONTROLLER_UTILS_HPP
#define CONTROLLER_UTILS_HPP

#include "httplib.h"
#include "nlohmann/json.hpp"
#include <optional>

#include "Result.hpp"
#include "typedefs.hpp"

namespace webapi::controllers {

std::optional<IDType> parse_id(const httplib::Request &req,
                               httplib::Response &res);

template <typename T>
std::optional<T> parse_entity_from_body(const httplib::Request &req,
                                        httplib::Response &res) {
  try {
    return nlohmann::json::parse(req.body).get<T>();
  } catch (const nlohmann::json::exception &e) {
    res.set_content(std::format("Failed to parse json: {}", e.what()),
                    "text/plain");
    res.status = httplib::StatusCode::BadRequest_400;
    return std::nullopt;
  }
}

// Handle cases where result.code is SQL_ERROR or ID_NOT_FOUND.
// Return true if `result.code != OK` to indicate the presence of an error.
template <typename T>
bool handle_result(const Result<T> &result, httplib::Response &res) {
  switch (result.code) {
  case OK:
    return false;
  case SQL_ERROR:
    res.status = httplib::StatusCode::InternalServerError_500;
    break;
  case ID_NOT_FOUND:
    res.status = httplib::StatusCode::NotFound_404;
    break;
  case AUTH_INVALID_TOKEN:
  case AUTH_BASIC_LOGIN_ONLY:
  case PASSWORD_VALIDATION_FAILED:
    res.status = httplib::StatusCode::BadRequest_400;
    break;
  case AUTH_WRONG_PASSWORD:
  case USER_NOT_FOUND:
    res.status = httplib::StatusCode::Unauthorized_401;
    break;
  default:
    res.set_content("Unknown server error", "text/plain");
    res.status = httplib::StatusCode::InternalServerError_500;
    return true;
  }

  res.set_content(result.message, "text/plain");
  return true;
}

} // namespace webapi::controllers

#endif