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
  if (result.code == SQL_ERROR) {
    res.set_content(result.message, "text/plain");
    res.status = httplib::StatusCode::InternalServerError_500;
    return true;
  }

  if (result.code == ID_NOT_FOUND) {
    res.set_content(result.message, "text/plain");
    res.status = httplib::StatusCode::NotFound_404;
    return true;
  }

  return false;
}

} // namespace webapi::controllers

#endif