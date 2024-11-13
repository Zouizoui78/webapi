#include "Repository.hpp"

#include <format>

namespace webapi::repositories {

Repository::Repository(db::SQLiteBackend *backend, std::string_view table_name,
                       const std::vector<std::string> &columns)
    : _backend(backend), _table_name(table_name) {
  _backend->create_table(_table_name, columns);
}

Result<bool> Repository::id_exists(IDType id) {
  auto res = _backend->query(
      std::format("SELECT id FROM {} WHERE id = {};", _table_name, id));
  if (res.code == SQL_ERROR) {
    return Result<bool>{.code = res.code, .message = res.message};
  }

  return Result<bool>{.value = res.code == OK && res.value == 1};
}

} // namespace webapi::repositories