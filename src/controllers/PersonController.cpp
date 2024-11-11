#include "PersonController.hpp"
#include "HTTPServer.hpp"

namespace webapi::controllers {

using namespace httplib;
using namespace nlohmann;
using namespace services;

PersonController::PersonController(PersonService *service)
    : IController(), _service(service) {}

std::string PersonController::get_name() const { return "person"; }

void PersonController::register_routes(HTTPServer *server) {
  server->register_get(get_name() + "/:id",
                       std::bind_front(&PersonController::read, this));
}

void PersonController::read(const httplib::Request &req,
                            httplib::Response &res) const {
  IDType id = 0;
  try {
    id = std::stoul(req.path_params.at("id"));
  } catch (std::invalid_argument &e) {
    res.set_content("Invalid id parameter", "text/plain");
    res.status = StatusCode::BadRequest_400;
    return;
  } catch (std::out_of_range &e) {
    res.set_content("Out of range id parameter", "text/plain");
    res.status = StatusCode::BadRequest_400;
    return;
  }

  auto p = _service->read(id);

  if (!p.has_value()) {
    res.status = StatusCode::NotFound_404;
    return;
  }

  res.set_content(json(*p).dump(), "application/json");
}

} // namespace webapi::controllers