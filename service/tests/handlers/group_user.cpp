#include "handlers/group_user.h"
#include "../mocs/dbmoc.hpp"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"
#include <gtest/gtest.h>

#include "pch.h"

namespace ymsummorizer::service {
using testing::Return;

class GroupUserHandlerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    log()->set_level(hare::hlevels::off);
    mock_db_impl_ = std::make_shared<storage::db_manager_mock>();

    auto impl = static_pointer_cast<storage::db::manager_base>(mock_db_impl_);
    db_manager_ = std::make_unique<storage::db_manager>(impl);
    handler_ = std::make_unique<handlers::group_user>(*db_manager_);
  }

  void TearDown() override {
    handler_.reset();
    mock_db_impl_.reset();
    db_manager_.reset();
  }

  std::shared_ptr<storage::db_manager_mock> mock_db_impl_;
  std::unique_ptr<storage::db_manager> db_manager_;
  std::unique_ptr<handlers::group_user> handler_;
};

TEST_F(GroupUserHandlerTest, Constructor) {
  EXPECT_NE(handler_, nullptr);
}

TEST_F(GroupUserHandlerTest, OnGroupUserAdd) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_user_add(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupUserHandlerTest, OnGroupUserRemove) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_user_remove(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupUserHandlerTest, OnGroupLeave) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_leave(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupUserHandlerTest, OnGroupUserAddWithGroup) {
  std::vector<common::group> groups;
  common::group group;
  group.id = "test_group";
  group.name = "TestGroup";
  groups.push_back(group);

  ON_CALL(*mock_db_impl_, get_stored_groups()).WillByDefault(Return(groups));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";

  auto result = handler_->on_group_user_add(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupUserHandlerTest, OnGroupUserRemoveWithGroup) {
  std::vector<common::group> groups;
  common::group group;
  group.id = "test_group";
  group.name = "TestGroup";
  groups.push_back(group);

  std::vector<common::user> users;
  common::user user;
  user.id = "test_user_id";
  user.login_tg = "test_user";
  user.login_yandex = "test_yandex";
  user.token = "test_token";
  users.push_back(user);

  ON_CALL(*mock_db_impl_, get_stored_groups()).WillByDefault(Return(groups));
  ON_CALL(*mock_db_impl_, get_stored_users()).WillByDefault(Return(users));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";

  auto result = handler_->on_group_user_remove(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupUserHandlerTest, OnGroupLeaveWithUser) {
  std::vector<common::group> groups;
  common::group group;
  group.id = "test_group";
  group.name = "TestGroup";
  group.user_ids = {"test_user_id"};
  groups.push_back(group);

  std::vector<common::user> users;
  common::user user;
  user.id = "test_user_id";
  user.login_tg = "test_user";
  user.login_yandex = "test_yandex";
  user.token = "test_token";
  users.push_back(user);

  ON_CALL(*mock_db_impl_, get_stored_groups()).WillByDefault(Return(groups));
  ON_CALL(*mock_db_impl_, get_stored_users()).WillByDefault(Return(users));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";

  auto result = handler_->on_group_leave(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupUserHandlerTest, EmptyInteraction) {
  tgbot::user_interaction ui;

  auto result_add = handler_->on_group_user_add(ui);
  auto result_remove = handler_->on_group_user_remove(ui);
  auto result_leave = handler_->on_group_leave(ui);

  EXPECT_NE(result_add, nullptr);
  EXPECT_NE(result_remove, nullptr);
  EXPECT_NE(result_leave, nullptr);
}
}  // namespace ymsummorizer::service
