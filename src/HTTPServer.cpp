#include <print>

#include "HTTPServer.hpp"

namespace webapi {

using namespace controllers;
using namespace httplib;

HTTPServer::HTTPServer(std::string_view web_ui_path) {
  set_mount_point("/", std::string(web_ui_path));

  set_pre_routing_handler([](const Request &req, Response &res) {
    std::println("http: {} {}", req.method, req.path);
    return Server::HandlerResponse::Unhandled;
  });

  set_post_routing_handler([](const Request &req, Response &res) {
    std::println("http: {} {} status = {}", req.method, req.path, res.status);
  });
}

bool HTTPServer::register_controller(
    std::unique_ptr<IController> &&controller) {
  if (_controllers.contains(controller->get_name())) {
    return false;
  }

  std::println("http: Registering controller '{}'", controller->get_name());
  controller->register_routes(this);
  _controllers.emplace(controller->get_name(),
                       std::forward<std::unique_ptr<IController>>(controller));
  return true;
}

void HTTPServer::register_get(std::string_view route, Server::Handler handler) {
  std::string full_route = std::format("/api/{}", route);
  std::println("http: Registering GET handler for route '{}'", full_route);
  Get(full_route, handler);
}

void HTTPServer::register_post(std::string_view route,
                               Server::Handler handler) {
  std::string full_route = std::format("/api/{}", route);
  std::println("http: Registering POST handler for route '{}'", full_route);
  Post(full_route, handler);
}

void HTTPServer::register_put(std::string_view route, Server::Handler handler) {
  std::string full_route = std::format("/api/{}", route);
  std::println("http: Registering PUT handler for route '{}'", full_route);
  Put(full_route, handler);
}

void HTTPServer::register_delete(std::string_view route,
                                 Server::Handler handler) {
  std::string full_route = std::format("/api/{}", route);
  std::println("http: Registering DELETE handler for route '{}'", full_route);
  Delete(full_route, handler);
}

} // namespace webapi
