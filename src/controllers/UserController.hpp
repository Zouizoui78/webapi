#ifndef USER_CONTROLLER_HPP
#define USER_CONTROLLER_HPP

#include "httplib.h"

#include "IController.hpp"
#include "services/UserService.hpp"

namespace webapi::controllers {

class UserController : public IController {
public:
  UserController(services::UserService *service);

  std::string get_name() const override;
  void register_routes(HTTPServer *server) override;

private:
  void create(const httplib::Request &req, httplib::Response &res) const;
  void read(const httplib::Request &req, httplib::Response &res) const;
  void update(const httplib::Request &req, httplib::Response &res) const;
  void remove(const httplib::Request &req, httplib::Response &res) const;

  services::UserService *_service = nullptr;
};

} // namespace webapi::controllers

#endif