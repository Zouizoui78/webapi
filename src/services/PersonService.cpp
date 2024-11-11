#include "PersonService.hpp"

namespace webapi::services {

using namespace repositories;

PersonService::PersonService(PersonRepository *repo) : _repo(repo) {}

std::optional<IDType> PersonService::create(const entities::Person &person) {
  return _repo->create(person);
}

std::optional<entities::Person> PersonService::read(IDType id) {
  return _repo->read(id);
}

bool PersonService::update(IDType id, const entities::Person &person) {
  return _repo->update(id, person);
}

bool PersonService::remove(IDType id) { return _repo->remove(id); }

} // namespace webapi::services