#ifndef USER_SERVICE_HPP
#define USER_SERVICE_HPP

#include <optional>

#include "Result.hpp"
#include "entities/User.hpp"
#include "repositories/UserRepository.hpp"
#include "typedefs.hpp"

namespace webapi::services {

class UserService final {
public:
  UserService(repositories::UserRepository *repo);

  Result<IDType> create(const entities::User &user);
  Result<entities::User> read(IDType id);
  Result<> update(IDType id, const entities::User &user);
  Result<> remove(IDType id);

private:
  repositories::UserRepository *_repo;
};

} // namespace webapi::services

#endif