#ifndef PERSON_REPOSITORY_HPP
#define PERSON_REPOSITORY_HPP

#include "Repository.hpp"
#include "Result.hpp"
#include "entities/Person.hpp"

namespace webapi::repositories {

class PersonRepository final : public Repository {
public:
  PersonRepository(db::SQLiteBackend *backend);

  Result<IDType> create(const entities::Person &person);
  Result<entities::Person> read(IDType id);
  Result<> update(IDType id, const entities::Person &person);
  Result<> remove(IDType id);
};

} // namespace webapi::repositories

#endif