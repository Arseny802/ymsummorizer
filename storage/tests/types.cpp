#include <gtest/gtest.h>

#include "storage/types.h"

namespace ymsummorizer::storage {

class storage_types_test : public ::testing::Test {};

TEST_F(storage_types_test, NoneValue) {
  EXPECT_EQ(static_cast<int>(storage_types::none), 0);
}

TEST_F(storage_types_test, CfgJsonValue) {
  EXPECT_EQ(static_cast<int>(storage_types::cfg_json), 1);
}

TEST_F(storage_types_test, Sqlite3Value) {
  EXPECT_EQ(static_cast<int>(storage_types::sqlite3), 2);
}

TEST_F(storage_types_test, AllValuesDistinct) {
  auto values = {storage_types::none, storage_types::cfg_json, storage_types::sqlite3};
  
  for (auto v1 : values) {
    for (auto v2 : values) {
      if (v1 != v2) {
        EXPECT_NE(static_cast<int>(v1), static_cast<int>(v2));
      }
    }
  }
}

}  // namespace ymsummorizer::storage
