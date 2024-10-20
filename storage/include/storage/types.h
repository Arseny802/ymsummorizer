#pragma once
#include <string>

namespace ymsummorizer::storage {
  enum class storage_types {
    none = 0,
    cfg_json,
    sqlite3,
  };

} // namespace ymsummorizer::storage