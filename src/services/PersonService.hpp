#ifndef PERSON_SERVICE_HPP
#define PERSON_SERVICE_HPP

#include <optional>

#include "Result.hpp"
#include "entities/Person.hpp"
#include "repositories/PersonRepository.hpp"
#include "typedefs.hpp"

namespace webapi::services {

class PersonService final {
public:
  PersonService(repositories::PersonRepository *repo);

  Result<IDType> create(const entities::Person &person);
  Result<entities::Person> read(IDType id);
  Result<> update(IDType id, const entities::Person &person);
  Result<> remove(IDType id);

private:
  repositories::PersonRepository *_repo;
};

} // namespace webapi::services

#endif