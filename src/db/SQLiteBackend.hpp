#ifndef SQLITE_BACKEND_HPP
#define SQLITE_BACKEND_HPP

#include <memory>
#include <print>
#include <string>
#include <vector>

#include "Result.hpp"
#include "sqlite3.h"
#include "typedefs.hpp"

namespace webapi::db {

class SQLiteBackend final {
public:
  SQLiteBackend(std::string_view db_path);
  SQLiteBackend(const SQLiteBackend &other) = delete;
  SQLiteBackend(SQLiteBackend &&other) = delete;
  SQLiteBackend &operator=(const SQLiteBackend &other) = delete;
  SQLiteBackend &operator=(SQLiteBackend &&other) = delete;

  bool create_table(std::string_view table_name,
                    const std::vector<std::string> &columns);

  // Return 0 if no insertion has been made since the creation of the connection
  // to the db.
  IDType get_last_inserted_id() const;

  std::string_view get_last_error() const;

  // Result.value contains the number of affected rows, if no error occured.
  template <typename T>
  Result<int> query(const std::string &statement, T &&callback) const {
    std::println("db: Running query '{}'", statement);
    sqlite3_stmt *stmt = nullptr;

    int res = sqlite3_prepare_v2(_handle.get(), statement.c_str(), -1, &stmt,
                                 nullptr);
    if (res != SQLITE_OK) {
      std::println("db: Failed to prepare SQL statement : {}",
                   sqlite3_errmsg(_handle.get()));
      return Result<int>{.code = SQL_ERROR, .message = messages::SQL_ERROR};
    }

    res = sqlite3_step(stmt);
    int count = 0;
    while (res == SQLITE_ROW) {
      callback(stmt);
      res = sqlite3_step(stmt);
      ++count;
    }

    if (res != SQLITE_DONE) {
      std::println("db: Sqlite statement '{}' encountered error on step #{} : ",
                   statement, count, sqlite3_errmsg(_handle.get()));
      return Result<int>{.code = SQL_ERROR, .message = messages::SQL_ERROR};
    }

    sqlite3_finalize(stmt);
    return Result<int>{.value = count};
  }

private:
  struct Sqlite3Deleter {
    void operator()(sqlite3 *handle);
  };

  std::unique_ptr<sqlite3, Sqlite3Deleter> _handle;
  char *_sqlite_error_buff;
  std::string _last_error;

  std::string _db_path;
};

} // namespace webapi::db

#endif