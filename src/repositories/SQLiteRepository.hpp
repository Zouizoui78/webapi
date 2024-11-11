#ifndef SQLITE_REPOSITORY_HPP
#define SQLITE_REPOSITORY_HPP

#include <memory>
#include <vector>

#include "IRepository.hpp"
#include "sqlite3.h"

namespace repositories {

class SQLiteRepository : public IRepository {
public:
  SQLiteRepository(std::string_view repo_name);

  IDType create(entities::Entity &&obj) override;
  entities::Entity read(IDType &&id) override;
  entities::Entity update(entities::Entity &&obj) override;
  bool remove(IDType &&id) override;

private:
  struct Sqlite3Deleter {
    void operator()(sqlite3 *handle);
  };

protected:
  static const sqlite3 *get_handle();

  // Get columns definitions as strings
  virtual std::vector<std::string> get_column_definitions() const = 0;

private:
  void create_table();

  // Return the number of affected rows, or -1 in case of error.
  template <typename T> int query(const std::string &statement, T &&callback);

  static std::unique_ptr<sqlite3, Sqlite3Deleter> _handle;
  static char *_sqlite_error_buff;

  static std::string _db_path;
  std::string _repo_name;
};

} // namespace repositories

#endif