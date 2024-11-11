#ifndef PERSON_REPOSITORY_HPP
#define PERSON_REPOSITORY_HPP

#include "IPersonRepository.hpp"
#include "db/SQLiteBackend.hpp"
#include "entities/Person.hpp"

namespace repositories {

class PersonRepository final : public IPersonRepository {
public:
  PersonRepository(db::SQLiteBackend *backend);

  std::optional<IDType> create(const entities::Person &person) override;
  std::optional<entities::Person> read(IDType id) override;
  bool update(IDType id, const entities::Person &person) override;
  bool remove(IDType id) override;

private:
  db::SQLiteBackend *_backend = nullptr;
  std::string _name = "person";
};

} // namespace repositories

#endif