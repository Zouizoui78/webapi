#include "SQLiteBackend.hpp"

#include <format>
#include <stdexcept>

namespace webapi::db {

SQLiteBackend::SQLiteBackend(std::string_view db_path) : _db_path(db_path) {
  if (_handle == nullptr) {
    std::string path(_db_path);
    sqlite3 *handle;
    int res = sqlite3_open(_db_path.c_str(), &handle);
    if (res != SQLITE_OK || handle == nullptr) {
      throw std::runtime_error(
          std::format("db: Failed to open database file {} : {}",
                      _db_path.c_str(), sqlite3_errmsg(handle)));
    }
    _handle = std::unique_ptr<sqlite3, Sqlite3Deleter>(handle);
    std::println("db: Opened database '{}'", _db_path);
  }
}

IDType SQLiteBackend::get_last_inserted_id() const {
  return sqlite3_last_insert_rowid(_handle.get());
}

bool SQLiteBackend::create_table(
    std::string_view table_name,
    const std::vector<std::string> &columns) const {
  std::string statement = std::format(
      "CREATE TABLE IF NOT EXISTS {} (id INTEGER PRIMARY KEY,", table_name);
  for (const std::string &column : columns) {
    statement += column + ",";
  }
  statement.pop_back();
  statement += ");";

  std::println("db: Creating table '{}' with statement '{}'", table_name,
               statement);

  char *error_msg;
  int res = sqlite3_exec(_handle.get(), statement.c_str(), nullptr, nullptr,
                         &error_msg);
  if (res != SQLITE_OK) {
    std::string msg(std::format("db: Failed to create table {} : {}",
                                table_name, error_msg));
    sqlite3_free(error_msg);
    throw std::runtime_error(msg);
  }

  return true;
}

void SQLiteBackend::Sqlite3Deleter::operator()(sqlite3 *handle) {
  int res = sqlite3_close(handle);

  if (res != SQLITE_OK) {
    std::println("db: Failed to close database");
    return;
  }
  std::println("db: Closed database");
}

} // namespace webapi::db