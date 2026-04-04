#include "handlers/user.h"
#include "storage/mocs/db_manager.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"
#include <gtest/gtest.h>

#include "pch.h"

namespace ymsummorizer::service {
using testing::Return;

class UserHandlerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    log()->set_level(hare::hlevels::off);
    mock_db_ = std::make_unique<storage::mocs::db_manager>(storage::storage_types::sqlite3, "test_db");
    handler_ = std::make_unique<handlers::user>(*mock_db_);
  }

  void TearDown() override {
    handler_.reset();
    mock_db_.reset();
  }

  std::unique_ptr<storage::mocs::db_manager> mock_db_;
  std::unique_ptr<handlers::user> handler_;
};

TEST_F(UserHandlerTest, Constructor) {
  EXPECT_NE(handler_, nullptr);
}

TEST_F(UserHandlerTest, OnStart) {
  ON_CALL(*mock_db_, get_stored_users()).WillByDefault(Return(std::vector<common::user>{}));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_start(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_FALSE(result->ok);
}

TEST_F(UserHandlerTest, OnStartWithUser) {
  std::vector<common::user> users;
  common::user test_user;
  test_user.id = "test_user_id";
  test_user.login_tg = "test_user";
  test_user.login_yandex = "test_yandex";
  test_user.token = "test_token";
  users.push_back(test_user);

  ON_CALL(*mock_db_, get_stored_users()).WillByDefault(Return(users));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_start(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(UserHandlerTest, OnUserTokenAdd) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_user_token_add(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_FALSE(result->ok);
}

TEST_F(UserHandlerTest, OnUserTokenErase) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_user_token_erase(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_FALSE(result->ok);
}

TEST_F(UserHandlerTest, OnTokenEdit) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_token_edit(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_FALSE(result->ok);
}

TEST_F(UserHandlerTest, OnUserView) {
  ON_CALL(*mock_db_, get_stored_users()).WillByDefault(Return(std::vector<common::user>{}));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_user_view(ui);
  EXPECT_NE(result, nullptr);
  EXPECT_FALSE(result->ok);
}

TEST_F(UserHandlerTest, OnUserViewWithExistingUser) {
  std::vector<common::user> users;
  common::user test_user;
  test_user.id = "test_user_id";
  test_user.login_tg = "test_user";
  test_user.login_yandex = "test_yandex";
  test_user.token = "test_token";
  users.push_back(test_user);

  ON_CALL(*mock_db_, get_stored_users()).WillByDefault(Return(users));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_user_view(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(UserHandlerTest, OnStartEmptyInteraction) {
  tgbot::user_interaction ui;

  auto result = handler_->on_start(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(UserHandlerTest, OnUserTokenAddWithToken) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;
  ui.arguments["token"] = "some_token";

  auto result = handler_->on_user_token_add(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(UserHandlerTest, MultipleInteractions) {
  ON_CALL(*mock_db_, get_stored_users()).WillByDefault(Return(std::vector<common::user>{}));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result1 = handler_->on_start(ui);
  auto result2 = handler_->on_user_view(ui);
  auto result3 = handler_->on_token_edit(ui);

  EXPECT_NE(result1, nullptr);
  EXPECT_NE(result2, nullptr);
  EXPECT_NE(result3, nullptr);
}

TEST_F(UserHandlerTest, OnUserViewWithAllFields) {
  std::vector<common::user> users;
  common::user test_user;
  test_user.id = "full_user";
  test_user.name = "Full User";
  test_user.login_tg = "tg_login";
  test_user.login_yandex = "yandex_login";
  test_user.token = "secret_token";
  users.push_back(test_user);

  ON_CALL(*mock_db_, get_stored_users()).WillByDefault(Return(users));

  tgbot::user_interaction ui;
  ui.user_login_tg = "tg_login";

  auto result = handler_->on_user_view(ui);
  EXPECT_NE(result, nullptr);
}
}  // namespace ymsummorizer::service
