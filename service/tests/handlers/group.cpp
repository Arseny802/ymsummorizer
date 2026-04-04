#include "handlers/group.h"
#include "../mocs/dbmoc.hpp"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"
#include <gtest/gtest.h>

#include "pch.h"

namespace ymsummorizer::service {
using testing::Return;

class GroupHandlerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    log()->set_level(hare::hlevels::off);
    mock_db_impl_ = std::make_shared<storage::db_manager_mock>();

    auto impl = static_pointer_cast<storage::db::manager_base>(mock_db_impl_);
    db_manager_ = std::make_unique<storage::db_manager>(impl);
    handler_ = std::make_unique<handlers::group>(*db_manager_);
  }

  void TearDown() override {
    handler_.reset();
    mock_db_impl_.reset();
    db_manager_.reset();
  }

  std::shared_ptr<storage::db_manager_mock> mock_db_impl_;
  std::unique_ptr<storage::db_manager> db_manager_;
  std::unique_ptr<handlers::group> handler_;
};

TEST_F(GroupHandlerTest, Constructor) {
  ON_CALL(*mock_db_impl_, connect(testing::_)).WillByDefault(Return(true));
  EXPECT_NE(handler_, nullptr);
}

TEST_F(GroupHandlerTest, OnGroupListEmpty) {
  ON_CALL(*mock_db_impl_, get_stored_groups()).WillByDefault(Return(std::vector<common::group>{}));
  ON_CALL(*mock_db_impl_, get_stored_users()).WillByDefault(Return(std::vector<common::user>{}));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_list(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_TRUE(result->ok);
  EXPECT_EQ(result->error_msg, "");
}

TEST_F(GroupHandlerTest, OnGroupListWithGroups) {
  std::vector<common::group> groups;
  common::group test_group;
  test_group.id = "test_group_id";
  test_group.name = "TestGroup";
  test_group.user_ids = {"user1"};
  groups.push_back(test_group);

  std::vector<common::user> users;
  common::user test_user;
  test_user.id = "user1";
  test_user.login_tg = "test_tg_login";
  test_user.login_yandex = "test_yandex_login";
  test_user.token = "test_token";
  users.push_back(test_user);

  ON_CALL(*mock_db_impl_, get_stored_groups()).WillByDefault(Return(groups));
  ON_CALL(*mock_db_impl_, get_stored_users()).WillByDefault(Return(users));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_list(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_TRUE(result->ok);
}

TEST_F(GroupHandlerTest, OnGroupCreateValidInput) {
  ON_CALL(*mock_db_impl_, add_group(testing::_)).WillByDefault(Return(true));

  tgbot::user_interaction ui;
  ui.arguments[tgbot::user_interaction::key_group_name] = "NewTestGroup";
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_create(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_TRUE(result->ok);
}

TEST_F(GroupHandlerTest, OnGroupCreateEmptyName) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_create(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_FALSE(result->ok);
}

TEST_F(GroupHandlerTest, OnGroupCreateWithSpecialChars) {
  ON_CALL(*mock_db_impl_, add_group(testing::_)).WillByDefault(Return(true));

  tgbot::user_interaction ui;
  ui.arguments[tgbot::user_interaction::key_group_name] = "Test_Group-123";
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_create(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_TRUE(result->ok);
}

TEST_F(GroupHandlerTest, OnGroupDelete) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_delete(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_TRUE(result->ok);
}

TEST_F(GroupHandlerTest, OnGroupListMultipleGroups) {
  std::vector<common::group> groups;
  for (int i = 0; i < 3; ++i) {
    common::group group;
    group.id = "group_" + std::to_string(i);
    group.name = "Group " + std::to_string(i);
    groups.push_back(group);
  }

  ON_CALL(*mock_db_impl_, get_stored_groups()).WillByDefault(Return(groups));
  ON_CALL(*mock_db_impl_, get_stored_users()).WillByDefault(Return(std::vector<common::user>{}));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";

  auto result = handler_->on_group_list(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_TRUE(result->ok);
}

TEST_F(GroupHandlerTest, OnGroupListWithUsersInGroup) {
  std::vector<common::user> users;
  common::user user1;
  user1.id = "user1";
  user1.login_tg = "tg1";
  users.push_back(user1);

  common::user user2;
  user2.id = "user2";
  user2.login_tg = "tg2";
  users.push_back(user2);

  std::vector<common::group> groups;
  common::group group;
  group.id = "group1";
  group.name = "TestGroup";
  group.user_ids = {"user1", "user2"};
  groups.push_back(group);

  ON_CALL(*mock_db_impl_, get_stored_groups()).WillByDefault(Return(groups));
  ON_CALL(*mock_db_impl_, get_stored_users()).WillByDefault(Return(users));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";

  auto result = handler_->on_group_list(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_TRUE(result->ok);
}

TEST_F(GroupHandlerTest, OnGroupCreateLongName) {
  ON_CALL(*mock_db_impl_, add_group(testing::_)).WillByDefault(Return(true));

  tgbot::user_interaction ui;
  ui.arguments[tgbot::user_interaction::key_group_name] = std::string(100, 'A');
  ui.user_login_tg = "test_user";

  auto result = handler_->on_group_create(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_TRUE(result->ok);
}

TEST_F(GroupHandlerTest, OnGroupListEmptyUsers) {
  std::vector<common::group> groups;
  common::group group;
  group.id = "empty_group";
  group.name = "EmptyGroup";
  groups.push_back(group);

  ON_CALL(*mock_db_impl_, get_stored_groups()).WillByDefault(Return(groups));
  ON_CALL(*mock_db_impl_, get_stored_users()).WillByDefault(Return(std::vector<common::user>{}));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";

  auto result = handler_->on_group_list(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_TRUE(result->ok);
}

TEST_F(GroupHandlerTest, OnGroupCreateAddGroupFailure) {
  ON_CALL(*mock_db_impl_, add_group(testing::_)).WillByDefault(Return(false));

  tgbot::user_interaction ui;
  ui.arguments[tgbot::user_interaction::key_group_name] = "NewTestGroup";
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_create(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_FALSE(result->ok);
}
}  // namespace ymsummorizer::service
