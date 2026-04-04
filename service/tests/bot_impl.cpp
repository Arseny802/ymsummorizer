#include "bot_impl.h"
#include "gmock/gmock.h"
#include <gmock/gmock.h>
// #include "dbmoc.h"
#include "../mocs/dbmoc.hpp"
#include <gtest/gtest.h>

#include "pch.h"
#include "storage/storage.hpp"
#include "tgbot/tgbot.hpp"

namespace ymsummorizer::service {
using testing::Return;

class BotImplTest : public ::testing::Test {
 protected:
  void SetUp() override {
    log()->set_level(hare::hlevels::off);
    tgbot::get_logger()->set_level(hare::hlevels::off);
    storage::get_logger()->set_level(hare::hlevels::off);
    mock_db_impl_ = std::make_shared<storage::db_manager_mock>();
    bot_info_.token = "TEST_TOKEN";
    ON_CALL(*mock_db_impl_, get_bot_info()).WillByDefault(Return(bot_info_));

    auto impl = static_pointer_cast<storage::db::manager_base>(mock_db_impl_);
    db_manager_ = std::make_unique<storage::db_manager>(std::move(impl));
    bot_ = std::make_unique<bot_impl>(std::move(*db_manager_));
  }

  void TearDown() override {
    bot_.reset();
    mock_db_impl_.reset();
    db_manager_.reset();
  }

  std::shared_ptr<storage::db_manager_mock> mock_db_impl_;
  std::unique_ptr<storage::db_manager> db_manager_;
  std::unique_ptr<bot_impl> bot_;

  common::bot_info bot_info_;
};

TEST_F(BotImplTest, Constructor) {
  EXPECT_NE(bot_, nullptr);
}

TEST_F(BotImplTest, InheritsFromHandlers) {
  // Проверяем, что bot_impl наследуется от всех handlers
  EXPECT_NE(dynamic_cast<handlers::user*>(bot_.get()), nullptr);
  EXPECT_NE(dynamic_cast<handlers::group*>(bot_.get()), nullptr);
  EXPECT_NE(dynamic_cast<handlers::group_playlist*>(bot_.get()), nullptr);
  EXPECT_NE(dynamic_cast<handlers::group_user*>(bot_.get()), nullptr);
}

}  // namespace ymsummorizer::service
