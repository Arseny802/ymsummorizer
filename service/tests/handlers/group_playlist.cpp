#include "handlers/group_playlist.h"
#include "../mocs/dbmoc.hpp"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"
#include <gtest/gtest.h>

#include "pch.h"

namespace ymsummorizer::service {
using testing::Return;

class GroupPlaylistHandlerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    log()->set_level(hare::hlevels::off);
    mock_db_impl_ = std::make_shared<storage::db_manager_mock>();

    auto impl = static_pointer_cast<storage::db::manager_base>(mock_db_impl_);
    db_manager_ = std::make_unique<storage::db_manager>(impl);
    handler_ = std::make_unique<handlers::group_playlist>(*db_manager_);
  }

  void TearDown() override {
    handler_.reset();
    mock_db_impl_.reset();
    db_manager_.reset();
  }

  std::shared_ptr<storage::db_manager_mock> mock_db_impl_;
  std::unique_ptr<storage::db_manager> db_manager_;
  std::unique_ptr<handlers::group_playlist> handler_;
};

TEST_F(GroupPlaylistHandlerTest, Constructor) {
  EXPECT_NE(handler_, nullptr);
}

TEST_F(GroupPlaylistHandlerTest, OnGroupPlaylistList) {
  ON_CALL(*mock_db_impl_, get_group_playlists(testing::_, testing::_))
      .WillByDefault(Return(std::vector<common::playlist>{}));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_playslit_list(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupPlaylistHandlerTest, OnGroupPlaylistView) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_playslit_view(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupPlaylistHandlerTest, OnGroupPlaylistAdd) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_playslit_add(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupPlaylistHandlerTest, OnGroupPlaylistRemove) {
  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";
  ui.timestamp = 1234567890;

  auto result = handler_->on_group_playslit_remove(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupPlaylistHandlerTest, OnGroupPlaylistListWithGroup) {
  std::vector<common::group> groups;
  common::group group;
  group.id = "test_group";
  group.name = "TestGroup";
  groups.push_back(group);

  ON_CALL(*mock_db_impl_, get_stored_groups()).WillByDefault(Return(groups));
  ON_CALL(*mock_db_impl_, get_group_playlists(testing::_, testing::_))
      .WillByDefault(Return(std::vector<common::playlist>{}));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";

  auto result = handler_->on_group_playslit_list(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupPlaylistHandlerTest, OnGroupPlaylistListEmptyInteraction) {
  ON_CALL(*mock_db_impl_, get_group_playlists(testing::_, testing::_))
      .WillByDefault(Return(std::vector<common::playlist>{}));

  tgbot::user_interaction ui;

  auto result = handler_->on_group_playslit_list(ui);
  EXPECT_NE(result, nullptr);
}

TEST_F(GroupPlaylistHandlerTest, OnGroupPlaylistViewWithPlaylist) {
  std::vector<common::playlist> playlists;
  common::playlist playlist;
  playlist.id = "playlist_1";
  playlist.group_id = "test_group";
  playlist.name = "TestPlaylist";
  playlists.push_back(playlist);

  ON_CALL(*mock_db_impl_, get_group_playlists(testing::_, testing::_)).WillByDefault(Return(playlists));

  tgbot::user_interaction ui;
  ui.user_login_tg = "test_user";

  auto result = handler_->on_group_playslit_view(ui);
  EXPECT_NE(result, nullptr);
}
}  // namespace ymsummorizer::service
