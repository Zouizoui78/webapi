#include "UserRepository.hpp"

#include <format>

namespace webapi::repositories {

using namespace entities;

UserRepository::UserRepository(db::SQLiteBackend *backend)
    : Repository(backend, "user", {"name STRING", "admin INTEGER"}) {}

Result<IDType> UserRepository::create(const User &user) {
  std::string statement =
      std::format("INSERT INTO {} "
                  "(name, admin) "
                  "VALUES('{}', '{}');",
                  _table_name, user.name, static_cast<int>(user.admin));

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

  std::string statement =
      std::format("UPDATE {} "
                  "SET name = '{}', admin = '{}' "
                  "WHERE id = {};",
                  _table_name, user.name, static_cast<int>(user.admin), id);

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

} // namespace webapi::repositories