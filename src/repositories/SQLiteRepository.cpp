#include "SQLiteRepository.hpp"

#include <format>
#include <iostream>
#include <print>
#include <stdexcept>

namespace repositories {

std::string SQLiteRepository::_db_path("test.db");

SQLiteRepository::SQLiteRepository(std::string_view repo_name)
    : _repo_name(repo_name) {
  if (_handle == nullptr) {
    std::string path(_db_path);
    sqlite3 *handle;
    int res = sqlite3_open(_db_path.c_str(), &handle);
    if (res != SQLITE_OK || handle == nullptr) {
      throw std::runtime_error(
          std::format("Failed to open database file {} : {}", _db_path.c_str(),
                      sqlite3_errmsg(handle)));
    }
    _handle = std::unique_ptr<sqlite3, Sqlite3Deleter>(handle);
  }

  create_table();
}

const sqlite3 *SQLiteRepository::get_handle() { return _handle.get(); }

void SQLiteRepository::create_table() {
  std::string statement = "CREATE TABLE IF NOT EXISTS " + _repo_name + " (";
  for (const std::string &column : get_column_definitions()) {
    statement += column + ",";
  }
  statement.pop_back();
  statement += ");";

  std::print("Creating table {} with statement {}", _repo_name, statement);

  int res = sqlite3_exec(_handle.get(), statement.c_str(), nullptr, nullptr,
                         &_sqlite_error_buff);
  if (res != SQLITE_OK) {
    throw std::runtime_error(
        std::format("Failed to create table test : {}", _sqlite_error_buff));
    sqlite3_free(_sqlite_error_buff);
  }
}

template <typename T>
int SQLiteRepository::query(const std::string &statement, T &&callback) {
  sqlite3_stmt *stmt = nullptr;

  int res =
      sqlite3_prepare_v2(_handle.get(), statement.c_str(), -1, &stmt, nullptr);
  if (res != SQLITE_OK) {
    std::cout << "Failed to prepare SQL statement : "
              << sqlite3_errmsg(_handle.get()) << std::endl;
    return -1;
  }

  res = sqlite3_step(stmt);
  int count = 0;
  while (res == SQLITE_ROW) {
    callback(stmt);
    res = sqlite3_step(stmt);
    ++count;
  }

  if (res != SQLITE_DONE) {
    std::cout << "Sqlite statement '" << statement
              << "' encountered error on step #" << count << " : "
              << sqlite3_errmsg(_handle.get()) << std::endl;
    return -1;
  }

  sqlite3_finalize(stmt);
  return count;
}

void SQLiteRepository::Sqlite3Deleter::operator()(sqlite3 *handle) {
  int res = sqlite3_close(handle);
  if (res != SQLITE_OK) {
    std::cout << "Failed to close database\n";
  }
}

} // namespace repositories
