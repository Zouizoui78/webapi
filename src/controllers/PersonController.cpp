#include "nlohmann/json.hpp"

#include "HTTPServer.hpp"
#include "PersonController.hpp"
#include "utils.hpp"

namespace webapi::controllers {

using namespace entities;
using namespace httplib;
using namespace nlohmann;
using namespace services;

PersonController::PersonController(PersonService *service)
    : IController(), _service(service) {}

std::string PersonController::get_name() const { return "person"; }

void PersonController::register_routes(HTTPServer *server) {
  std::string user_route(get_name() + "/:id");
  server->register_post(get_name(),
                        std::bind_front(&PersonController::create, this));
  server->register_get(user_route,
                       std::bind_front(&PersonController::read, this));
  server->register_put(user_route,
                       std::bind_front(&PersonController::update, this));
  server->register_delete(user_route,
                          std::bind_front(&PersonController::remove, this));
}

void PersonController::create(const httplib::Request &req,
                              httplib::Response &res) const {
  std::optional<Person> p = parse_entity_from_body<Person>(req, res);
  if (!p.has_value()) {
    return;
  }

  std::optional<IDType> p_id = _service->create(*p);
  if (!p_id.has_value()) {
    res.set_content("Failed to create person entity", "text/plain");
    res.status = StatusCode::BadRequest_400;
    return;
  }

  p->id = *p_id;
  res.set_content(json(*p).dump(), "application/json");
}

void PersonController::read(const httplib::Request &req,
                            httplib::Response &res) const {
  std::optional<IDType> id = parse_id(req, res);
  if (!id.has_value()) {
    return;
  }

  auto p = _service->read(*id);
  if (!p.has_value()) {
    res.status = StatusCode::NotFound_404;
    return;
  }

  res.set_content(json(*p).dump(), "application/json");
}

void PersonController::update(const httplib::Request &req,
                              httplib::Response &res) const {
  std::optional<IDType> id = parse_id(req, res);
  if (!id.has_value()) {
    return;
  }

  std::optional<Person> p = parse_entity_from_body<Person>(req, res);
  if (!p.has_value()) {
    return;
  }

  if (!_service->update(*id, *p)) {
    res.set_content(std::format("Failed to update person entity {}", *id),
                    "text/plain");
    res.status = StatusCode::BadRequest_400;
    return;
  }
}

void PersonController::remove(const httplib::Request &req,
                              httplib::Response &res) const {
  std::optional<IDType> id = parse_id(req, res);
  if (!id.has_value()) {
    return;
  }

  if (!_service->remove(*id)) {
    res.status = StatusCode::NotFound_404;
  }
}

} // namespace webapi::controllers