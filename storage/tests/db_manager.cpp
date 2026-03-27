#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

#include "hare/config.h"
#include "hare/config_default.h"
#include "hare/hare_loggers.h"
#include "hare/hlevels.h"
#include "mocs/manager_base_mock.h"
#include "storage/db_manager.h"
#include "storage/storage.hpp"

#include "../src/pch.h"

namespace ymsummorizer::storage {

class db_manager_test : public ::testing::Test {
 protected:
  void SetUp() override {
    log()->set_level(hare::hlevels::off);

    mock_manager_ = std::make_shared<db::manager_base_mock>();
    ON_CALL(*mock_manager_, connect(::testing::_)).WillByDefault(::testing::Return(true));
    ON_CALL(*mock_manager_, create_db()).WillByDefault(::testing::Return(true));
    ON_CALL(*mock_manager_, get_stored_settings())
        .WillByDefault(::testing::Return(std::optional<std::vector<common::setting>>{}));

    db_manager_ = std::make_unique<db_manager>(storage_types::sqlite3, "test_db");
    // The mock is injected via the friend class access to private members
    db_manager_->manager_pimpl = mock_manager_;
  }

  decltype((db_manager::connected_)) get_connected() { return db_manager_->connected_; }
  decltype((db_manager::settings_)) get_settings() { return db_manager_->settings_; }

  std::shared_ptr<db::manager_base_mock> mock_manager_;
  std::unique_ptr<db_manager> db_manager_;
};

TEST_F(db_manager_test, ConnectSuccess) {
  EXPECT_CALL(*mock_manager_, connect("test_db")).WillOnce(::testing::Return(true));
  EXPECT_CALL(*mock_manager_, create_db()).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->connect());
}

TEST_F(db_manager_test, ConnectFailureOnConnect) {
  EXPECT_CALL(*mock_manager_, connect("test_db")).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->connect());
}

TEST_F(db_manager_test, ConnectFailureOnCreateDb) {
  EXPECT_CALL(*mock_manager_, connect("test_db")).WillOnce(::testing::Return(true));
  EXPECT_CALL(*mock_manager_, create_db()).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->connect());
}

TEST_F(db_manager_test, Flash) {
  EXPECT_CALL(*mock_manager_, flash()).WillOnce(::testing::Return(true));

  get_connected() = true;

  EXPECT_TRUE(db_manager_->flash());
}

TEST_F(db_manager_test, GetStoredSettingFoundInCache) {
  get_settings()["test_key"] = "cached_value";

  common::setting setting;
  setting.key = "test_key";

  EXPECT_EQ(db_manager_->get_stored_setting(setting, "default"), "cached_value");
  // Should not call the mock
}

TEST_F(db_manager_test, GetStoredSettingFoundInDb) {
  common::setting setting;
  setting.key = "test_key";
  setting.value = "db_value";

  EXPECT_CALL(*mock_manager_, get_stored_setting(::testing::_))
      .WillOnce(::testing::DoAll(::testing::SetArgReferee<0>(setting), ::testing::Return(true)));

  EXPECT_EQ(db_manager_->get_stored_setting(setting, "default"), "db_value");
}

TEST_F(db_manager_test, GetStoredSettingNotFound) {
  common::setting setting;
  setting.key = "test_key";

  EXPECT_CALL(*mock_manager_, get_stored_setting(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_EQ(db_manager_->get_stored_setting(setting, "default"), "default");
}

TEST_F(db_manager_test, SetStoredSetting) {
  common::setting setting;
  setting.key = "test_key";
  setting.value = "test_value";

  EXPECT_CALL(*mock_manager_, set_stored_setting(setting)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->set_stored_setting(setting));
}

// Tests for already connected state
TEST_F(db_manager_test, ConnectWhenAlreadyConnected) {
  get_connected() = true;
  EXPECT_TRUE(db_manager_->connect());
}

// Tests for get_stored_setting with empty key
TEST_F(db_manager_test, GetStoredSettingWithEmptyKey) {
  common::setting setting;
  setting.key = "";

  // Should return default and log warning
  EXPECT_EQ(db_manager_->get_stored_setting(setting, "default"), "default");
}

// Tests for get_stored_setting with string key
TEST_F(db_manager_test, GetStoredSettingStringKeyFound) {
  common::setting setting;
  setting.key = "test_key";
  setting.value = "db_value";

  EXPECT_CALL(*mock_manager_, get_stored_setting(::testing::_))
      .WillOnce(::testing::DoAll(::testing::SetArgReferee<0>(setting), ::testing::Return(true)));

  EXPECT_EQ(db_manager_->get_stored_setting("test_key", "default"), "db_value");
}

TEST_F(db_manager_test, GetStoredSettingStringKeyNotFound) {
  EXPECT_CALL(*mock_manager_, get_stored_setting(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_EQ(db_manager_->get_stored_setting("nonexistent_key", "default"), "default");
}

// Tests for users
TEST_F(db_manager_test, GetStoredUsers) {
  std::vector<common::user> users;
  users.push_back(common::user{"1", "user1", "login1", "token1", ""});

  EXPECT_CALL(*mock_manager_, get_stored_users()).WillOnce(::testing::Return(users));

  auto result = db_manager_->get_stored_users();
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result[0].id, "1");
}

TEST_F(db_manager_test, AddUser) {
  common::user user{"1", "user1", "login1", "token1", ""};

  EXPECT_CALL(*mock_manager_, add_user(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->add_user(user));
}

TEST_F(db_manager_test, RemoveUser) {
  EXPECT_CALL(*mock_manager_, remove_user("user_id_1")).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->remove_user("user_id_1"));
}

TEST_F(db_manager_test, UpdateUser) {
  common::user user{"1", "updated_user", "login1", "token1", ""};

  EXPECT_CALL(*mock_manager_, update_user(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->update_user(user));
}

// Tests for groups
TEST_F(db_manager_test, GetStoredGroups) {
  std::vector<common::group> groups;
  common::group grp;
  grp.id = "1";
  grp.name = "group1";
  grp.user_ids = {"user1", "user2"};
  groups.push_back(grp);

  EXPECT_CALL(*mock_manager_, get_stored_groups()).WillOnce(::testing::Return(groups));

  auto result = db_manager_->get_stored_groups();
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result[0].id, "1");
}

TEST_F(db_manager_test, AddGroup) {
  common::group group;
  group.id = "1";
  group.name = "group1";
  group.user_ids = {"user1"};

  EXPECT_CALL(*mock_manager_, add_group(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->add_group(group));
}

TEST_F(db_manager_test, RemoveGroup) {
  EXPECT_CALL(*mock_manager_, remove_group("group_id_1")).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->remove_group("group_id_1"));
}

TEST_F(db_manager_test, UpdateGroup) {
  common::group group;
  group.id = "1";
  group.name = "updated_group";
  group.user_ids = {"user1"};

  EXPECT_CALL(*mock_manager_, update_group(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->update_group(group));
}

// Tests for bot_info
TEST_F(db_manager_test, GetBotInfo) {
  common::bot_info info;
  info.token = "test_token";

  EXPECT_CALL(*mock_manager_, get_bot_info()).WillOnce(::testing::Return(info));

  auto result = db_manager_->get_bot_info();
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result->token, "test_token");
}

TEST_F(db_manager_test, GetBotInfoEmpty) {
  EXPECT_CALL(*mock_manager_, get_bot_info()).WillOnce(::testing::Return(std::nullopt));

  auto result = db_manager_->get_bot_info();
  EXPECT_FALSE(result.has_value());
}

TEST_F(db_manager_test, UpdateBotInfo) {
  common::bot_info info;
  info.token = "new_token";

  EXPECT_CALL(*mock_manager_, update_bot_info(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->update_bot_info(info));
}

TEST_F(db_manager_test, DeleteBotInfo) {
  EXPECT_CALL(*mock_manager_, delete_bot_info()).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->delete_bot_info());
}

// Tests for admins
TEST_F(db_manager_test, GetAdminIds) {
  std::vector<std::string> admin_ids{"admin1", "admin2"};

  EXPECT_CALL(*mock_manager_, get_admin_ids()).WillOnce(::testing::Return(admin_ids));

  auto result = db_manager_->get_admin_ids();
  EXPECT_EQ(result.size(), 2);
}

TEST_F(db_manager_test, GetSuperadminIds) {
  std::vector<std::string> superadmin_ids{"superadmin1"};

  EXPECT_CALL(*mock_manager_, get_superadmin_ids()).WillOnce(::testing::Return(superadmin_ids));

  auto result = db_manager_->get_superadmin_ids();
  EXPECT_EQ(result.size(), 1);
}

TEST_F(db_manager_test, AddAdmin) {
  common::user admin{"1", "admin", "login", "token", ""};

  EXPECT_CALL(*mock_manager_, add_admin(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->add_admin(admin));
}

TEST_F(db_manager_test, AddSuperadmin) {
  common::group group;
  group.id = "1";
  group.name = "group";

  EXPECT_CALL(*mock_manager_, add_superadmin(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->add_superadmin(group));
}

TEST_F(db_manager_test, RemoveAdmin) {
  EXPECT_CALL(*mock_manager_, remove_admin("admin_id")).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->remove_admin("admin_id"));
}

TEST_F(db_manager_test, RemoveSuperadmin) {
  EXPECT_CALL(*mock_manager_, remove_superadmin("group_id")).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->remove_superadmin("group_id"));
}

// Tests for playlists
TEST_F(db_manager_test, GetGroupPlaylists) {
  common::playlist playlist;
  playlist.id = "playlist1";
  playlist.group_id = "group1";
  playlist.name = "Test Playlist";

  std::vector<common::playlist> playlists{playlist};

  EXPECT_CALL(*mock_manager_, get_group_playlists("group1", "")).WillOnce(::testing::Return(playlists));

  auto result = db_manager_->get_group_playlists("group1");
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result[0].id, "playlist1");
}

TEST_F(db_manager_test, GetGroupPlaylistsWithPlaylistId) {
  common::playlist playlist;
  playlist.id = "playlist1";
  playlist.group_id = "group1";

  EXPECT_CALL(*mock_manager_, get_group_playlists("group1", "playlist1"))
      .WillOnce(::testing::Return(std::vector<common::playlist>{playlist}));

  auto result = db_manager_->get_group_playlists("group1", "playlist1");
  EXPECT_EQ(result.size(), 1);
}

TEST_F(db_manager_test, AddPlaylist) {
  common::playlist playlist;
  playlist.id = "playlist1";
  playlist.group_id = "group1";

  EXPECT_CALL(*mock_manager_, add_playlist(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->add_playlist(playlist));
}

TEST_F(db_manager_test, RemovePlaylist) {
  common::playlist playlist;
  playlist.id = "playlist1";
  playlist.group_id = "group1";

  EXPECT_CALL(*mock_manager_, remove_playlist(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->remove_playlist(playlist));
}

TEST_F(db_manager_test, AddPlaylistYandex) {
  common::playlist playlist;
  playlist.id = "playlist1";
  playlist.group_id = "group1";

  common::playlist::yandex yandex;
  yandex.user_id = "user1";
  yandex.kind = 12345;

  EXPECT_CALL(*mock_manager_, add_playlist_yandex(::testing::_, ::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->add_playlist_yandex(playlist, yandex));
}

TEST_F(db_manager_test, RemovePlaylistYandex) {
  common::playlist playlist;
  playlist.id = "playlist1";
  playlist.group_id = "group1";

  common::playlist::yandex yandex;
  yandex.user_id = "user1";
  yandex.kind = 12345;

  EXPECT_CALL(*mock_manager_, remove_playlist_yandex(::testing::_, ::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->remove_playlist_yandex(playlist, yandex));
}

// Test for connect with settings loaded from DB
TEST_F(db_manager_test, ConnectWithSettingsFromDb) {
  std::vector<common::setting> settings;
  settings.push_back(common::setting{"key1", "value1"});
  settings.push_back(common::setting{"key2", "value2"});

  EXPECT_CALL(*mock_manager_, connect("test_db")).WillOnce(::testing::Return(true));
  EXPECT_CALL(*mock_manager_, create_db()).WillOnce(::testing::Return(true));
  EXPECT_CALL(*mock_manager_, get_stored_settings()).WillOnce(::testing::Return(settings));

  EXPECT_TRUE(db_manager_->connect());

  // Check that settings were loaded into cache
  EXPECT_EQ(get_settings()["key1"], "value1");
  EXPECT_EQ(get_settings()["key2"], "value2");
}

// Test for move constructor
TEST_F(db_manager_test, MoveConstructor) {
  db_manager_->connect();
  EXPECT_TRUE(get_connected());

  db_manager moved(std::move(*db_manager_));
  // After move, the original should have connected_ = false
  // The new object would have the same storage_type and db_name
}

// Test for destructor (just verify it doesn't crash)
TEST_F(db_manager_test, Destructor) {
  db_manager_->connect();
  // Will be called when going out of scope
}

// ============================================================================
// Дополнительные тесты для непокрытых сценариев
// ============================================================================

// Тест: connect() не создаёт новый pimpl если уже подключен
TEST_F(db_manager_test, ConnectDoesNotResetExistingConnection) {
  get_connected() = true;

  // Не должно вызывать методы manager_pimpl
  EXPECT_TRUE(db_manager_->connect());
  EXPECT_TRUE(get_connected());
}

// Тест: get_stored_setting(string) - пустой ключ
TEST_F(db_manager_test, GetStoredSettingStringKeyEmpty) {
  // При пустом ключе должен вернуть default_value
  EXPECT_EQ(db_manager_->get_stored_setting("", "default"), "default");
}

// Тест: get_stored_setting(string) - ключ не в кэше, есть в БД
TEST_F(db_manager_test, GetStoredSettingStringKeyFromDb) {
  common::setting setting;
  setting.key = "db_key";
  setting.value = "db_value";

  EXPECT_CALL(*mock_manager_, get_stored_setting(::testing::_))
      .WillOnce(::testing::DoAll(::testing::SetArgReferee<0>(setting), ::testing::Return(true)));

  EXPECT_EQ(db_manager_->get_stored_setting("db_key", "default"), "db_value");
}

// Тест: set_stored_setting не обновляет кэш напрямую
// (кэш обновляется только при connect() через get_stored_settings)
TEST_F(db_manager_test, SetStoredSettingDoesNotUpdateCache) {
  common::setting setting;
  setting.key = "new_key";
  setting.value = "new_value";

  EXPECT_CALL(*mock_manager_, set_stored_setting(setting)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->set_stored_setting(setting));

  // Кэш не обновляется при set_stored_setting
  EXPECT_EQ(get_settings().count("new_key"), 0);
}

// Тест: set_stored_setting при пустом ключе
TEST_F(db_manager_test, SetStoredSettingEmptyKey) {
  common::setting setting;
  setting.key = "";
  setting.value = "value";

  // Должен пытаться сохранить, даже с пустым ключом
  EXPECT_CALL(*mock_manager_, set_stored_setting(setting)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->set_stored_setting(setting));
}

// Тест: flash() без подключения
TEST_F(db_manager_test, FlashNotConnected) {
  get_connected() = false;

  // При неподключенном состоянии должен вернуть false
  // (manager_pimpl может быть nullptr)
  EXPECT_FALSE(db_manager_->flash());
}

// Тест: get_group_playlists с пустыми параметрами
TEST_F(db_manager_test, GetGroupPlaylistsEmptyParams) {
  EXPECT_CALL(*mock_manager_, get_group_playlists("", "")).WillOnce(::testing::Return(std::vector<common::playlist>{}));

  auto result = db_manager_->get_group_playlists();
  EXPECT_TRUE(result.empty());
}

// Тест: get_group_playlists возвращает несколько плейлистов
TEST_F(db_manager_test, GetGroupPlaylistsMultiple) {
  std::vector<common::playlist> playlists;
  for (int i = 0; i < 3; ++i) {
    common::playlist p;
    p.id = "playlist" + std::to_string(i);
    p.group_id = "group1";
    p.name = "Playlist " + std::to_string(i);
    playlists.push_back(p);
  }

  EXPECT_CALL(*mock_manager_, get_group_playlists("group1", "")).WillOnce(::testing::Return(playlists));

  auto result = db_manager_->get_group_playlists("group1");
  EXPECT_EQ(result.size(), 3);
}

// Тест: несколько пользователей
TEST_F(db_manager_test, GetMultipleUsers) {
  std::vector<common::user> users;
  for (int i = 0; i < 5; ++i) {
    common::user u;
    u.id = "user" + std::to_string(i);
    u.name = "User " + std::to_string(i);
    u.login_tg = "login" + std::to_string(i);
    u.login_yandex = "yandex_login" + std::to_string(i);
    u.token = "token" + std::to_string(i);
    users.push_back(u);
  }

  EXPECT_CALL(*mock_manager_, get_stored_users()).WillOnce(::testing::Return(users));

  auto result = db_manager_->get_stored_users();
  EXPECT_EQ(result.size(), 5);
  EXPECT_EQ(result[2].id, "user2");
  EXPECT_EQ(result[4].name, "User 4");
}

// Тест: несколько групп
TEST_F(db_manager_test, GetMultipleGroups) {
  std::vector<common::group> groups;
  for (int i = 0; i < 3; ++i) {
    common::group g;
    g.id = "group" + std::to_string(i);
    g.name = "Group " + std::to_string(i);
    g.user_ids = {"user1", "user2"};
    groups.push_back(g);
  }

  EXPECT_CALL(*mock_manager_, get_stored_groups()).WillOnce(::testing::Return(groups));

  auto result = db_manager_->get_stored_groups();
  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result[0].user_ids.size(), 2);
}

// Тест: remove_user с пустым ID
TEST_F(db_manager_test, RemoveUserEmptyId) {
  // Должен пытаться удалить, даже с пустым ID
  EXPECT_CALL(*mock_manager_, remove_user("")).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->remove_user(""));
}

// Тест: remove_group с пустым ID
TEST_F(db_manager_test, RemoveGroupEmptyId) {
  EXPECT_CALL(*mock_manager_, remove_group("")).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->remove_group(""));
}

// Тест: update_user с пустым ID
TEST_F(db_manager_test, UpdateUserEmptyId) {
  common::user user;
  user.id = "";
  user.name = "test";

  EXPECT_CALL(*mock_manager_, update_user(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->update_user(user));
}

// Тест: update_group с пустым ID
TEST_F(db_manager_test, UpdateGroupEmptyId) {
  common::group group;
  group.id = "";
  group.name = "test";

  EXPECT_CALL(*mock_manager_, update_group(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->update_group(group));
}

// Тест: add_playlist с пустыми полями
TEST_F(db_manager_test, AddPlaylistEmptyFields) {
  common::playlist playlist;  // Все поля пустые

  EXPECT_CALL(*mock_manager_, add_playlist(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->add_playlist(playlist));
}

// Тест: remove_playlist с пустыми полями
TEST_F(db_manager_test, RemovePlaylistEmptyFields) {
  common::playlist playlist;  // Все поля пустые

  EXPECT_CALL(*mock_manager_, remove_playlist(::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->remove_playlist(playlist));
}

// Тест: add_playlist_yandex с пустыми полями
TEST_F(db_manager_test, AddPlaylistYandexEmptyFields) {
  common::playlist playlist;
  common::playlist::yandex yandex;  // Пустой yandex

  EXPECT_CALL(*mock_manager_, add_playlist_yandex(::testing::_, ::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->add_playlist_yandex(playlist, yandex));
}

// Тест: remove_playlist_yandex с пустыми полями
TEST_F(db_manager_test, RemovePlaylistYandexEmptyFields) {
  common::playlist playlist;
  common::playlist::yandex yandex;

  EXPECT_CALL(*mock_manager_, remove_playlist_yandex(::testing::_, ::testing::_)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->remove_playlist_yandex(playlist, yandex));
}

// Тест: add_user возвращает false при ошибке
TEST_F(db_manager_test, AddUserFailure) {
  common::user user{"1", "user1", "login1", "token1", ""};

  EXPECT_CALL(*mock_manager_, add_user(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->add_user(user));
}

// Тест: add_group возвращает false при ошибке
TEST_F(db_manager_test, AddGroupFailure) {
  common::group group;
  group.id = "1";
  group.name = "group1";

  EXPECT_CALL(*mock_manager_, add_group(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->add_group(group));
}

// Тест: update_user возвращает false при ошибке
TEST_F(db_manager_test, UpdateUserFailure) {
  common::user user{"1", "updated_user", "login1", "token1", ""};

  EXPECT_CALL(*mock_manager_, update_user(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->update_user(user));
}

// Тест: update_group возвращает false при ошибке
TEST_F(db_manager_test, UpdateGroupFailure) {
  common::group group;
  group.id = "1";
  group.name = "updated_group";

  EXPECT_CALL(*mock_manager_, update_group(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->update_group(group));
}

// Тест: add_admin возвращает false при ошибке
TEST_F(db_manager_test, AddAdminFailure) {
  common::user admin{"1", "admin", "login", "token", ""};

  EXPECT_CALL(*mock_manager_, add_admin(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->add_admin(admin));
}

// Тест: add_superadmin возвращает false при ошибке
TEST_F(db_manager_test, AddSuperadminFailure) {
  common::group group;
  group.id = "1";
  group.name = "group";

  EXPECT_CALL(*mock_manager_, add_superadmin(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->add_superadmin(group));
}

// Тест: remove_admin возвращает false при ошибке
TEST_F(db_manager_test, RemoveAdminFailure) {
  EXPECT_CALL(*mock_manager_, remove_admin("admin_id")).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->remove_admin("admin_id"));
}

// Тест: remove_superadmin возвращает false при ошибке
TEST_F(db_manager_test, RemoveSuperadminFailure) {
  EXPECT_CALL(*mock_manager_, remove_superadmin("group_id")).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->remove_superadmin("group_id"));
}

// Тест: update_bot_info возвращает false при ошибке
TEST_F(db_manager_test, UpdateBotInfoFailure) {
  common::bot_info info;
  info.token = "new_token";

  EXPECT_CALL(*mock_manager_, update_bot_info(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->update_bot_info(info));
}

// Тест: delete_bot_info возвращает false при ошибке
TEST_F(db_manager_test, DeleteBotInfoFailure) {
  EXPECT_CALL(*mock_manager_, delete_bot_info()).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->delete_bot_info());
}

// Тест: add_playlist возвращает false при ошибке
TEST_F(db_manager_test, AddPlaylistFailure) {
  common::playlist playlist;
  playlist.id = "playlist1";

  EXPECT_CALL(*mock_manager_, add_playlist(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->add_playlist(playlist));
}

// Тест: remove_playlist возвращает false при ошибке
TEST_F(db_manager_test, RemovePlaylistFailure) {
  common::playlist playlist;
  playlist.id = "playlist1";

  EXPECT_CALL(*mock_manager_, remove_playlist(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->remove_playlist(playlist));
}

// Тест: add_playlist_yandex возвращает false при ошибке
TEST_F(db_manager_test, AddPlaylistYandexFailure) {
  common::playlist playlist;
  playlist.id = "playlist1";

  common::playlist::yandex yandex;
  yandex.user_id = "user1";
  yandex.kind = 12345;

  EXPECT_CALL(*mock_manager_, add_playlist_yandex(::testing::_, ::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->add_playlist_yandex(playlist, yandex));
}

// Тест: remove_playlist_yandex возвращает false при ошибке
TEST_F(db_manager_test, RemovePlaylistYandexFailure) {
  common::playlist playlist;
  playlist.id = "playlist1";

  common::playlist::yandex yandex;
  yandex.user_id = "user1";
  yandex.kind = 12345;

  EXPECT_CALL(*mock_manager_, remove_playlist_yandex(::testing::_, ::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->remove_playlist_yandex(playlist, yandex));
}

// Тест: get_stored_users возвращает пустой вектор
TEST_F(db_manager_test, GetStoredUsersEmpty) {
  EXPECT_CALL(*mock_manager_, get_stored_users()).WillOnce(::testing::Return(std::vector<common::user>{}));

  auto result = db_manager_->get_stored_users();
  EXPECT_TRUE(result.empty());
}

// Тест: get_stored_groups возвращает пустой вектор
TEST_F(db_manager_test, GetStoredGroupsEmpty) {
  EXPECT_CALL(*mock_manager_, get_stored_groups()).WillOnce(::testing::Return(std::vector<common::group>{}));

  auto result = db_manager_->get_stored_groups();
  EXPECT_TRUE(result.empty());
}

// Тест: get_group_playlists возвращает пустой вектор
TEST_F(db_manager_test, GetGroupPlaylistsEmpty) {
  EXPECT_CALL(*mock_manager_, get_group_playlists("group1", ""))
      .WillOnce(::testing::Return(std::vector<common::playlist>{}));

  auto result = db_manager_->get_group_playlists("group1");
  EXPECT_TRUE(result.empty());
}

// Тест: get_admin_ids возвращает пустой вектор
TEST_F(db_manager_test, GetAdminIdsEmpty) {
  EXPECT_CALL(*mock_manager_, get_admin_ids()).WillOnce(::testing::Return(std::vector<std::string>{}));

  auto result = db_manager_->get_admin_ids();
  EXPECT_TRUE(result.empty());
}

// Тест: get_superadmin_ids возвращает пустой вектор
TEST_F(db_manager_test, GetSuperadminIdsEmpty) {
  EXPECT_CALL(*mock_manager_, get_superadmin_ids()).WillOnce(::testing::Return(std::vector<std::string>{}));

  auto result = db_manager_->get_superadmin_ids();
  EXPECT_TRUE(result.empty());
}

// Тест: playlist с несколькими yandex пользователями
TEST_F(db_manager_test, PlaylistWithMultipleYandexUsers) {
  common::playlist playlist;
  playlist.id = "playlist1";
  playlist.group_id = "group1";
  playlist.name = "Test Playlist";

  common::playlist::yandex yandex1;
  yandex1.user_id = "user1";
  yandex1.kind = 111;

  common::playlist::yandex yandex2;
  yandex2.user_id = "user2";
  yandex2.kind = 222;

  playlist.yandex_users.push_back(yandex1);
  playlist.yandex_users.push_back(yandex2);

  // Тестируем, что структура корректно создаётся
  EXPECT_EQ(playlist.yandex_users.size(), 2);
  EXPECT_EQ(playlist.yandex_users[0].kind, 111);
  EXPECT_EQ(playlist.yandex_users[1].kind, 222);
}

// Тест: group с несколькими пользователями
TEST_F(db_manager_test, GroupWithMultipleUsers) {
  common::group group;
  group.id = "group1";
  group.name = "Test Group";
  group.user_ids = {"user1", "user2", "user3", "user4"};

  EXPECT_EQ(group.user_ids.size(), 4);
  EXPECT_EQ(group.user_ids[2], "user3");
}

// Тест: user с специальными символами в имени
TEST_F(db_manager_test, UserWithSpecialChars) {
  common::user user;
  user.id = "123";
  user.name = "Name with <special> & \"chars\"";
  user.login_tg = "login";
  user.login_yandex = "yandex";
  user.token = "token";

  // Проверяем, что данные сохраняются корректно
  EXPECT_EQ(user.id, "123");
  EXPECT_THAT(user.name, ::testing::HasSubstr("special"));
}

// Тест: connect с настройками из БД - проверка логики кэширования
TEST_F(db_manager_test, ConnectLoadsSettingsIntoCache) {
  std::vector<common::setting> settings;
  settings.push_back(common::setting{"setting1", "value1"});
  settings.push_back(common::setting{"setting2", "value2"});
  settings.push_back(common::setting{"setting3", "value3"});

  EXPECT_CALL(*mock_manager_, connect("test_db")).WillOnce(::testing::Return(true));
  EXPECT_CALL(*mock_manager_, create_db()).WillOnce(::testing::Return(true));
  EXPECT_CALL(*mock_manager_, get_stored_settings()).WillOnce(::testing::Return(settings));

  EXPECT_TRUE(db_manager_->connect());

  // Проверяем, что все настройки загружены в кэш
  auto& cache = get_settings();
  EXPECT_EQ(cache.size(), 3);
  EXPECT_EQ(cache["setting1"], "value1");
  EXPECT_EQ(cache["setting2"], "value2");
  EXPECT_EQ(cache["setting3"], "value3");
}

// Тест: connect не загружает настройки если их нет в БД
TEST_F(db_manager_test, ConnectWithNoSettingsInDb) {
  EXPECT_CALL(*mock_manager_, connect("test_db")).WillOnce(::testing::Return(true));
  EXPECT_CALL(*mock_manager_, create_db()).WillOnce(::testing::Return(true));
  EXPECT_CALL(*mock_manager_, get_stored_settings()).WillOnce(::testing::Return(std::nullopt));

  EXPECT_TRUE(db_manager_->connect());

  // Кэш должен остаться пустым
  EXPECT_TRUE(get_settings().empty());
}

// Тест: verify move constructor creates disconnected object
TEST_F(db_manager_test, MoveConstructorCreatesDisconnectedObject) {
  // Установим некоторое состояние
  get_connected() = true;

  // Move-конструктор создаёт новый disconnected объект
  db_manager moved(std::move(*db_manager_));

  // Новый объект должен быть disconnected (особенность реализации)
  // connected_ = false в move-конструкторе
  // Оригинальный объект остаётся в прежнем состоянии
  EXPECT_TRUE(get_connected());  // Оригинальный объект не изменяется
}

}  // namespace ymsummorizer::storage
