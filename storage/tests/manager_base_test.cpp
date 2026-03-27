#include <gtest/gtest.h>

#include "storage/storage.hpp"

#include "../src/pch.h"
#include "../src/db/manager_base.h"

namespace ymsummorizer::storage::db {

class manager_base_test : public ::testing::Test {
 protected:
  void SetUp() override {
    log()->set_level(hare::hlevels::off);
  }
};

// Тест, что manager_base - абстрактный класс
TEST_F(manager_base_test, IsAbstract) {
  EXPECT_TRUE(std::is_abstract_v<manager_base>);
}

// Тест, что manager_base имеет виртуальный деструктор
TEST_F(manager_base_test, HasVirtualDestructor) {
  EXPECT_TRUE(std::has_virtual_destructor_v<manager_base>);
}

// Тест, что manager_base_ptr - это shared_ptr<manager_base>
TEST_F(manager_base_test, ManagerBasePtrType) {
  EXPECT_TRUE((std::is_same_v<manager_base_ptr, std::shared_ptr<manager_base>>));
}

}  // namespace ymsummorizer::storage::db
