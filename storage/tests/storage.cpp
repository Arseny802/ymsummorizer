#include <gtest/gtest.h>

#include "storage/storage.hpp"

#include "../src/pch.h"

namespace ymsummorizer::storage {

class storage_test : public ::testing::Test {
 protected:
  void SetUp() override {
    log()->set_level(hare::hlevels::off);
  }
};

TEST_F(storage_test, GetLoggerReturnsValidLogger) {
  auto logger = get_logger();
  EXPECT_NE(logger, nullptr);
}

TEST_F(storage_test, GetLoggerReturnsSameLogger) {
  auto logger1 = get_logger();
  auto logger2 = get_logger();
  
  EXPECT_EQ(logger1.get(), logger2.get());
}

}  // namespace ymsummorizer::storage
