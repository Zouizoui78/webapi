#ifndef IREPOSITORY_HPP
#define IREPOSITORY_HPP

#include <cstdint>

#include "entities/Entity.hpp"

namespace repositories {

class IRepository {
public:
  virtual std::string get_name() const = 0;

  virtual IDType create(entities::Entity &&obj) = 0;
  virtual entities::Entity read(IDType &&id) = 0;
  virtual entities::Entity update(entities::Entity &&obj) = 0;
  virtual bool remove(IDType &&id) = 0;

  IRepository() = default;
  virtual ~IRepository() = default;
  IRepository(const IRepository &other) = delete;
  IRepository(IRepository &&other) = delete;
  IRepository &operator=(const IRepository &other) = delete;
  IRepository &operator=(IRepository &&other) = delete;
};

} // namespace repositories

#endif