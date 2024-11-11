#ifndef CONTROLLER_UTILS_HPP
#define CONTROLLER_UTILS_HPP

#include "httplib.h"
#include "nlohmann/json.hpp"
#include <optional>

#include "typedefs.hpp"

namespace webapi::controllers {

// Return 0 if parsing fails.
std::optional<IDType> parse_id(const httplib::Request &req,
                               httplib::Response &res);

template <typename T>
std::optional<T> parse_entity_from_body(const httplib::Request &req,
                                        httplib::Response &res) {
  try {
    return nlohmann::json::parse(req.body);
  } catch (const nlohmann::json::exception &e) {
    res.set_content(std::format("Failed to parse json: {}", e.what()),
                    "text/plain");
    res.status = httplib::StatusCode::BadRequest_400;
    return std::nullopt;
  }
}

} // namespace webapi::controllers

#endif