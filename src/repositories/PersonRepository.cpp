#include "PersonRepository.hpp"

#include <format>

namespace webapi::repositories {

using namespace entities;

PersonRepository::PersonRepository(db::SQLiteBackend *backend)
    : _backend(backend) {
  _backend->create_table(
      "person", {"firstname STRING", "lastname STRING", "age INTEGER"});
}

Result<IDType> PersonRepository::create(const Person &person) {
  std::string statement =
      std::format("INSERT INTO {} "
                  "(firstname, lastname, age) "
                  "VALUES('{}', '{}', '{}');",
                  _name, person.firstname, person.lastname, person.age);

  auto backend_res = _backend->query(statement, [](sqlite3_stmt *stmt) { ; });
  if (backend_res.code != OK) {
    return Result<IDType>{.code = backend_res.code,
                          .message = backend_res.message};
  }

  return Result<IDType>{.value = _backend->get_last_inserted_id()};
}

Result<Person> PersonRepository::read(IDType id) {
  Person p;
  std::string stmt = std::format("SELECT * FROM {} WHERE id = {};", _name, id);
  auto backend_res = _backend->query(stmt, [&p](sqlite3_stmt *stmt) {
    p.id = sqlite3_column_int64(stmt, 0);
    p.firstname = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    p.lastname = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    p.age = sqlite3_column_int(stmt, 3);
  });

  if (backend_res.code != OK) {
    return Result<Person>{.code = backend_res.code,
                          .message = backend_res.message};
  }

  if (backend_res.value == 0) {
    return Result<Person>{.code = ID_NOT_FOUND,
                          .message = messages::ID_NOT_FOUND};
  }

  return Result<Person>{.value = p};
}

Result<> PersonRepository::update(IDType id, const Person &person) {
  std::string statement = std::format(
      "UPDATE {} "
      "SET firstname = '{}', lastname = '{}', age = '{}' "
      "WHERE id = {};",
      _name, person.id, person.firstname, person.lastname, person.age, id);

  auto backend_res = _backend->query(statement, [](sqlite3_stmt *stmt) { ; });
  if (backend_res.code != OK) {
    return Result<>{.code = backend_res.code, .message = backend_res.message};
  }

  if (backend_res.value == 0) {
    return Result<>{.code = ID_NOT_FOUND, .message = messages::ID_NOT_FOUND};
  }

  return Result<>();
}

Result<> PersonRepository::remove(IDType id) {
  std::string statement = std::format("DELETE FROM {} "
                                      "WHERE id = {};",
                                      _name, id);

  auto backend_res = _backend->query(statement, [](sqlite3_stmt *stmt) { ; });
  if (backend_res.code != OK) {
    return Result<>{.code = backend_res.code, .message = backend_res.message};
  }

  if (backend_res.value == 0) {
    return Result<>{.code = ID_NOT_FOUND, .message = messages::ID_NOT_FOUND};
  }

  return Result<>();
}

} // namespace webapi::repositories