#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>

#include "storage/db_manager.h"
#include "storage/storage.hpp"

#include "../src/db/fabric.h"
#include "../src/pch.h"

namespace ymsummorizer::storage::db {

class fabric_test : public ::testing::Test { };

TEST_F(fabric_test, CreateSqliteManager) {
  auto manager = fabric::create(storage_types::sqlite3);
  EXPECT_NE(manager, nullptr);
}

TEST_F(fabric_test, CreateCfgJsonManager) {
  auto manager = fabric::create(storage_types::cfg_json);
  EXPECT_NE(manager, nullptr);
}

TEST_F(fabric_test, CreateUnknownTypeThrows) {
  EXPECT_THROW(fabric::create(storage_types::none), std::runtime_error);
}

TEST_F(fabric_test, DifferentInstancesAreIndependent) {
  auto manager1 = fabric::create(storage_types::sqlite3);
  auto manager2 = fabric::create(storage_types::sqlite3);

  EXPECT_NE(manager1.get(), manager2.get());
}

}  // namespace ymsummorizer::storage::db
