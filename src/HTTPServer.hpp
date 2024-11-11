#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "httplib.h"
#include <memory>
#include <string>
#include <unordered_map>

#include "controllers/IController.hpp"

class SQLiteBackend;

namespace webapi {

class HTTPServer : public httplib::Server {
public:
  HTTPServer(std::string_view web_ui_path);

  // Return false if a controller with the same name is already registered, true
  // otherwise.
  bool
  register_controller(std::unique_ptr<controllers::IController> &&controller);

  void register_get(std::string_view route, httplib::Server::Handler handler);
  void register_post(std::string_view route, httplib::Server::Handler handler);
  void register_put(std::string_view route, httplib::Server::Handler handler);
  void register_delete(std::string_view route,
                       httplib::Server::Handler handler);

private:
  std::unordered_map<std::string, std::unique_ptr<controllers::IController>>
      _controllers;
};

} // namespace webapi

#endif
