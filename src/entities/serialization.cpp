#include "Person.hpp"

namespace webapi::entities {

using namespace nlohmann;

void to_json(json &j, const Person &p) {
  j = json{{"id", p.id},
           {"firstname", p.firstname},
           {"lastname", p.lastname},
           {"age", p.age}};
}

void from_json(const json &j, Person &p) {
  if (j.contains("id")) {
    j.at("id").get_to(p.id);
  }

  j.at("firstname").get_to(p.firstname);
  j.at("lastname").get_to(p.lastname);
  j.at("age").get_to(p.age);
}

} // namespace webapi::entities