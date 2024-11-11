#include "PersonRepository.hpp"

#include <format>

namespace webapi::repositories {

using namespace entities;

PersonRepository::PersonRepository(db::SQLiteBackend *backend)
    : _backend(backend) {
  _backend->create_table(
      "person", {"firstname STRING", "lastname STRING", "age INTEGER"});
}

std::optional<IDType> PersonRepository::create(const entities::Person &person) {
  std::string statement =
      std::format("INSERT INTO {} "
                  "(firstname, lastname, age) "
                  "VALUES('{}', '{}', '{}');",
                  _name, person.firstname, person.lastname, person.age);
  int res = _backend->query(statement, [](sqlite3_stmt *stmt) { ; });

  if (res == -1) {
    return std::nullopt;
  }

  return _backend->get_table_last_id(_name);
}

std::optional<entities::Person> PersonRepository::read(IDType id) {
  Person p;
  std::string stmt = std::format("SELECT * FROM {} WHERE id = {};", _name, id);
  int res = _backend->query(stmt, [&p](sqlite3_stmt *stmt) {
    p.id = sqlite3_column_int64(stmt, 0);
    p.firstname = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    p.lastname = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    p.age = sqlite3_column_int(stmt, 3);
  });

  if (res == -1) {
    return std::nullopt;
  }

  return p;
}

bool PersonRepository::update(IDType id, const entities::Person &person) {
  std::string statement = std::format(
      "UPDATE {} "
      "SET id = '{}', firstname = '{}', lastname = '{}', age = '{}'"
      "WHERE id = {};",
      _name, person.id, person.firstname, person.lastname, person.age, id);

  int res = _backend->query(statement, [](sqlite3_stmt *stmt) { ; });
  return res != -1;
}

bool PersonRepository::remove(IDType id) {
  std::string statement = std::format("DELETE FROM {} "
                                      "WHERE id = {};",
                                      _name, id);

  int res = _backend->query(statement, [](sqlite3_stmt *stmt) { ; });
  return res != -1;
}

} // namespace webapi::repositories