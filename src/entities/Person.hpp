#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>

#include "typedefs.hpp"

namespace entities {

struct Person {
  IDType id = 0;
  std::string first_name = "";
  std::string last_name = "";
};

} // namespace entities

#endif