#ifndef PERSON_REPOSITORY_HPP
#define PERSON_REPOSITORY_HPP

#include "db/SQLiteBackend.hpp"
#include "entities/Person.hpp"

namespace webapi::repositories {

class PersonRepository final {
public:
  PersonRepository(db::SQLiteBackend *backend);

  std::optional<IDType> create(const entities::Person &person);
  std::optional<entities::Person> read(IDType id);
  bool update(IDType id, const entities::Person &person);
  bool remove(IDType id);

private:
  db::SQLiteBackend *_backend = nullptr;
  std::string _name = "person";
};

} // namespace webapi::repositories

#endif