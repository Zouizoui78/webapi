#include "Person.hpp"

namespace webapi::entities {

void to_json(nlohmann::json &j, const Person &p) {
  j = nlohmann::json{{"id", p.id},
                     {"firstname", p.firstname},
                     {"lastname", p.lastname},
                     {"age", p.age}};
}

void from_json(const nlohmann::json &j, Person &p) {
  if (j.contains("id")) {
    j.at("id").get_to(p.id);
  }

  j.at("firstname").get_to(p.firstname);
  j.at("lastname").get_to(p.lastname);
  j.at("age").get_to(p.age);
}

} // namespace webapi::entities