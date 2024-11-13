#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

#include <string>
#include <vector>

#include "Result.hpp"
#include "db/SQLiteBackend.hpp"
#include "typedefs.hpp"

namespace webapi::repositories {

class Repository {
public:
  Repository(db::SQLiteBackend *backend, std::string_view table_name,
             const std::vector<std::string> &columns);

  Result<bool> id_exists(IDType id);

protected:
  const db::SQLiteBackend *_backend = nullptr;
  const std::string _table_name;
};

} // namespace webapi::repositories

#endif