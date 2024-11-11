#ifndef PERSON_HPP
#define PERSON_HPP

#include "nlohmann/json.hpp"
#include <string>

#include "typedefs.hpp"

namespace webapi::entities {

struct Person {
  IDType id = 0;
  std::string firstname = "";
  std::string lastname = "";
  int age = 0;
};

void to_json(nlohmann::json &j, const Person &p);
void from_json(const nlohmann::json &j, Person &p);

} // namespace webapi::entities

#endif