#ifndef PERSON_HPP
#define PERSON_HPP

#include "nlohmann/json.hpp"
#include <string>

#include "typedefs.hpp"

namespace webapi::entities {

struct Person {
  IDType id = 0;
  std::string first_name = "";
  std::string last_name = "";
  int age = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Person, id, first_name, last_name, age);

} // namespace webapi::entities

#endif