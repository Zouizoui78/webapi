#include "User.hpp"

namespace webapi::entities {

using namespace nlohmann;

void to_json(json &j, const User &p) {
  j = json{{"id", p.id}, {"name", p.name}, {"admin", p.admin}};
}

void from_json(const json &j, User &p) {
  if (j.contains("id")) {
    j.at("id").get_to(p.id);
  }

  j.at("name").get_to(p.name);
  j.at("admin").get_to(p.admin);
}

} // namespace webapi::entities