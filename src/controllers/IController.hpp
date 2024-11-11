#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP

#include <string>

namespace webapi {
class HTTPServer;
}

namespace webapi::controllers {

class IController {
public:
  virtual std::string get_name() const = 0;
  virtual void register_routes(webapi::HTTPServer *server) = 0;

  IController() = default;
  virtual ~IController() = default;
  IController(const IController &other) = delete;
  IController(IController &&other) = delete;
  IController &operator=(const IController &other) = delete;
  IController &operator=(IController &&other) = delete;
};

} // namespace webapi::controllers

#endif