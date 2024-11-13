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

  if (j.contains("password")) {
    std::string plain = j.at("password");
    std::size_t hash = std::hash<std::string>{}(plain);
    p.password = std::to_string(hash);
  }

  j.at("name").get_to(p.name);
  j.at("admin").get_to(p.admin);
}

} // namespace webapi::entities