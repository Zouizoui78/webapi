#ifndef USER_REPOSITORY_HPP
#define USER_REPOSITORY_HPP

#include "Repository.hpp"
#include "Result.hpp"
#include "entities/User.hpp"

namespace webapi::repositories {

class UserRepository final : public Repository {
public:
  UserRepository(db::SQLiteBackend *backend);

  Result<IDType> create(const entities::User &user);
  Result<entities::User> read(IDType id);
  Result<> update(IDType id, const entities::User &user);
  Result<> remove(IDType id);
};

} // namespace webapi::repositories

#endif