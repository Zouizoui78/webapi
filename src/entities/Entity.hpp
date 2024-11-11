#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "typedefs.hpp"

namespace entities {

class Entity {
public:
  IDType get_id() const { return _id; }

private:
  IDType _id;

public:
  virtual ~Entity() = default;
  Entity(const Entity &other) = delete;
  Entity(Entity &&other) = delete;
  Entity &operator=(const Entity &other) = delete;
  Entity &operator=(Entity &&other) = delete;
};

} // namespace entities

#endif