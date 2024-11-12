#include "PersonService.hpp"

namespace webapi::services {

using namespace repositories;

PersonService::PersonService(PersonRepository *repo) : _repo(repo) {}

Result<IDType> PersonService::create(const entities::Person &person) {
  return _repo->create(person);
}

Result<entities::Person> PersonService::read(IDType id) {
  return _repo->read(id);
}

Result<> PersonService::update(IDType id, const entities::Person &person) {
  return _repo->update(id, person);
}

Result<> PersonService::remove(IDType id) { return _repo->remove(id); }

} // namespace webapi::services