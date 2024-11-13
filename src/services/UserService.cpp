#include "UserService.hpp"

namespace webapi::services {

using namespace repositories;

UserService::UserService(UserRepository *repo) : _repo(repo) {}

Result<IDType> UserService::create(const entities::User &user) {
  return _repo->create(user);
}

Result<entities::User> UserService::read(IDType id) { return _repo->read(id); }

Result<> UserService::update(IDType id, const entities::User &user) {
  return _repo->update(id, user);
}

Result<> UserService::remove(IDType id) { return _repo->remove(id); }

Result<std::string> UserService::get_user_password(std::string_view name) {
  return _repo->get_user_password(name);
}

} // namespace webapi::services