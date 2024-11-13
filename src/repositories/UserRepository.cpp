#include "UserRepository.hpp"

#include <format>

namespace webapi::repositories {

using namespace entities;

UserRepository::UserRepository(db::SQLiteBackend *backend)
    : Repository(backend, "user",
                 {"name TEXT", "password TEXT", "admin INTEGER"}) {}

Result<IDType> UserRepository::create(const User &user) {
  if (user.password.empty()) {
    return Result<IDType>{.code = PASSWORD_VALIDATION_FAILED,
                          .message = messages::PASSWORD_VALIDATION_FAILED};
  }

  std::string statement = std::format("INSERT INTO {} "
                                      "(name, password, admin) "
                                      "VALUES('{}', '{}', '{}');",
                                      _table_name, user.name, user.password,
                                      static_cast<int>(user.admin));

  auto backend_res = _backend->query(statement);
  if (backend_res.code != OK) {
    return Result<IDType>{.code = backend_res.code,
                          .message = backend_res.message};
  }

  return Result<IDType>{.value = _backend->get_last_inserted_id()};
}

Result<User> UserRepository::read(IDType id) {
  User p;
  std::string stmt =
      std::format("SELECT * FROM {} WHERE id = {};", _table_name, id);
  auto backend_res = _backend->query(stmt, [&p](sqlite3_stmt *stmt) {
    p.id = sqlite3_column_int64(stmt, 0);
    p.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    p.admin = sqlite3_column_int(stmt, 2);
  });

  if (backend_res.code != OK) {
    return Result<User>{.code = backend_res.code,
                        .message = backend_res.message};
  }

  if (backend_res.value == 0) {
    return Result<User>{.code = ID_NOT_FOUND,
                        .message = messages::ID_NOT_FOUND};
  }

  return Result<User>{.value = p};
}

Result<> UserRepository::update(IDType id, const User &user) {
  auto id_exists_res = id_exists(id);
  if (id_exists_res.code == SQL_ERROR) {
    return Result<>{.code = id_exists_res.code,
                    .message = id_exists_res.message};
  }

  if (id_exists_res.code == OK && id_exists_res.value == false) {
    return Result<>{.code = ID_NOT_FOUND, .message = messages::ID_NOT_FOUND};
  }

  std::string password = user.password.empty()
                             ? get_user_password(user.name).value.value()
                             : user.password;

  std::string statement = std::format(
      "UPDATE {} "
      "SET name = '{}', password = '{}', admin = '{}' "
      "WHERE id = {};",
      _table_name, user.name, password, static_cast<int>(user.admin), id);

  auto backend_res = _backend->query(statement);
  if (backend_res.code != OK) {
    return Result<>{.code = backend_res.code, .message = backend_res.message};
  }

  return Result<>();
}

Result<> UserRepository::remove(IDType id) {
  auto id_exists_res = id_exists(id);
  if (id_exists_res.code == SQL_ERROR) {
    return Result<>{.code = id_exists_res.code,
                    .message = id_exists_res.message};
  }

  if (id_exists_res.code == OK && id_exists_res.value == false) {
    return Result<>{.code = ID_NOT_FOUND, .message = messages::ID_NOT_FOUND};
  }

  std::string statement = std::format("DELETE FROM {} "
                                      "WHERE id = {};",
                                      _table_name, id);

  auto backend_res = _backend->query(statement);
  if (backend_res.code != OK) {
    return Result<>{.code = backend_res.code, .message = backend_res.message};
  }

  return Result<>();
}

Result<std::string> UserRepository::get_user_password(std::string_view name) {
  std::string password;
  std::string query = std::format("SELECT password FROM {} WHERE name = '{}';",
                                  _table_name, name);
  auto backend_res = _backend->query(query, [&password](sqlite3_stmt *stmt) {
    const char *p =
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    password = std::string(
        reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
  });
  if (backend_res.code != OK) {
    return Result<std::string>{.code = backend_res.code,
                               .message = backend_res.message};
  }

  if (password.empty()) {
    return Result<std::string>{.code = USER_NOT_FOUND,
                               .message = messages::USER_NOT_FOUND};
  }

  return Result<std::string>{.value = password};
}

} // namespace webapi::repositories