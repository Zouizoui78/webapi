#include "nlohmann/json.hpp"

#include "HTTPServer.hpp"
#include "Result.hpp"
#include "UserController.hpp"
#include "utils.hpp"

namespace webapi::controllers {

using namespace entities;
using namespace httplib;
using namespace nlohmann;
using namespace services;

UserController::UserController(UserService *service)
    : IController(), _service(service) {}

std::string UserController::get_name() const { return "user"; }

void UserController::register_routes(HTTPServer *server) {
  std::string user_route(get_name() + "/:id");
  server->register_post(get_name(),
                        std::bind_front(&UserController::create, this));
  server->register_get(user_route,
                       std::bind_front(&UserController::read, this));
  server->register_put(user_route,
                       std::bind_front(&UserController::update, this));
  server->register_delete(user_route,
                          std::bind_front(&UserController::remove, this));
}

void UserController::create(const httplib::Request &req,
                            httplib::Response &res) const {
  std::optional<User> p = parse_entity_from_body<User>(req, res);
  if (!p.has_value()) {
    return;
  }

  auto result = _service->create(*p);
  if (handle_result(result, res)) {
    return;
  }

  p->id = result.value.value();
  res.set_content(json(*p).dump(), "application/json");
}

void UserController::read(const httplib::Request &req,
                          httplib::Response &res) const {
  std::optional<IDType> id = parse_id(req, res);
  if (!id.has_value()) {
    return;
  }

  auto result = _service->read(*id);
  if (handle_result(result, res)) {
    return;
  }

  res.set_content(json(result.value.value()).dump(), "application/json");
}

void UserController::update(const httplib::Request &req,
                            httplib::Response &res) const {
  std::optional<IDType> id = parse_id(req, res);
  if (!id.has_value()) {
    return;
  }

  std::optional<User> p = parse_entity_from_body<User>(req, res);
  if (!p.has_value()) {
    return;
  }

  auto result = _service->update(*id, *p);
  handle_result(result, res);
}

void UserController::remove(const httplib::Request &req,
                            httplib::Response &res) const {
  std::optional<IDType> id = parse_id(req, res);
  if (!id.has_value()) {
    return;
  }

  auto result = _service->remove(*id);
  handle_result(result, res);
}

} // namespace webapi::controllers