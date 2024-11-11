#ifndef PERSON_CONTROLLER_HPP
#define PERSON_CONTROLLER_HPP

#include "httplib.h"

#include "IController.hpp"
#include "services/PersonService.hpp"

namespace webapi::controllers {

class PersonController : public IController {
public:
  PersonController(services::PersonService *service);

  std::string get_name() const override;
  void register_routes(HTTPServer *server) override;

private:
  void create(const httplib::Request &req, httplib::Response &res) const;
  void read(const httplib::Request &req, httplib::Response &res) const;
  void update(const httplib::Request &req, httplib::Response &res) const;
  void remove(const httplib::Request &req, httplib::Response &res) const;

  services::PersonService *_service = nullptr;
};

} // namespace webapi::controllers

#endif