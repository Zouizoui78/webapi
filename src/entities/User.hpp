#ifndef USER_HPP
#define USER_HPP

#include "nlohmann/json.hpp"
#include <string>

#include "typedefs.hpp"

namespace webapi::entities {

struct User {
  IDType id = 0;
  std::string name = "";
  bool admin = false;
};

void to_json(nlohmann::json &j, const User &p);
void from_json(const nlohmann::json &j, User &p);

} // namespace webapi::entities

#endif