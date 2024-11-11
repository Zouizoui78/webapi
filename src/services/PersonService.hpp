#ifndef PERSON_SERVICE_HPP
#define PERSON_SERVICE_HPP

#include <optional>

#include "entities/Person.hpp"
#include "repositories/PersonRepository.hpp"
#include "typedefs.hpp"

namespace webapi::services {

class PersonService final {
public:
  PersonService(repositories::PersonRepository *repo);

  std::optional<IDType> create(const entities::Person &person);
  std::optional<entities::Person> read(IDType id);
  bool update(IDType id, const entities::Person &person);
  bool remove(IDType id);

private:
  repositories::PersonRepository *_repo;
};

} // namespace webapi::services

#endif