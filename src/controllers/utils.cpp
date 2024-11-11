#include "utils.hpp"

namespace webapi::controllers {

using namespace httplib;

std::optional<IDType> parse_id(const httplib::Request &req,
                               httplib::Response &res) {
  IDType id = 0;

  try {
    id = std::stoul(req.path_params.at("id"));
  } catch (std::exception &e) {
    res.set_content("Invalid or missing id parameter", "text/plain");
    res.status = StatusCode::BadRequest_400;
    return std::nullopt;
  }

  return id;
}

} // namespace webapi::controllers