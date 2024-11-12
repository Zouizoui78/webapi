#ifndef PERSON_REPOSITORY_HPP
#define PERSON_REPOSITORY_HPP

#include "Result.hpp"
#include "db/SQLiteBackend.hpp"
#include "entities/Person.hpp"

namespace webapi::repositories {

class PersonRepository final {
public:
  PersonRepository(db::SQLiteBackend *backend);

  Result<IDType> create(const entities::Person &person);
  Result<entities::Person> read(IDType id);
  Result<> update(IDType id, const entities::Person &person);
  Result<> remove(IDType id);

private:
  db::SQLiteBackend *_backend = nullptr;
  std::string _name = "person";
};

} // namespace webapi::repositories

#endif