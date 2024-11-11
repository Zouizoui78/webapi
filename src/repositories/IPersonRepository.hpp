#ifndef IPERSON_REPOSITORY_HPP
#define IPERSON_REPOSITORY_HPP

#include <cstdint>
#include <optional>

#include "entities/Person.hpp"

namespace webapi::repositories {

class IPersonRepository {
public:
  virtual std::optional<IDType> create(const entities::Person &person) = 0;
  virtual std::optional<entities::Person> read(IDType id) = 0;
  virtual bool update(IDType id, const entities::Person &person) = 0;
  virtual bool remove(IDType id) = 0;

  IPersonRepository() = default;
  ~IPersonRepository() = default;
  IPersonRepository(const IPersonRepository &other) = delete;
  IPersonRepository(IPersonRepository &&other) = delete;
  IPersonRepository &operator=(const IPersonRepository &other) = delete;
  IPersonRepository &operator=(IPersonRepository &&other) = delete;
};

} // namespace webapi::repositories

#endif