#include "pch.h"
#include "fabric.h"
#include "sqlite/manager.h"

namespace ymsummorizer::storage::db {
  manager_base_ptr fabric::create(storage_types storage_type) {
    AUTOLOG_ST
    switch (storage_type) {
    case storage_types::sqlite3: return std::make_shared<sqlite::manager>();
    default: throw std::runtime_error("unknown storage type");
    }
  }
} // namespace ymsummorizer::storage::db
