#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../db_manager.h"

namespace ymsummorizer::storage::mocs {

class db_manager : protected ymsummorizer::storage::db_manager {
 public:
  db_manager(storage_types storage_type, std::string db_name)
      : ymsummorizer::storage::db_manager(storage_type, std::move(db_name)) { }
  ~db_manager();

  // Мок-методы для connect/flash
  MOCK_METHOD(bool, connect, (), (override));
  MOCK_METHOD(bool, flash, (), (override));

  // Мок-методы для settings
  MOCK_METHOD(std::string, get_stored_setting_string_key, (const std::string&, std::string), (override));
  MOCK_METHOD(std::string, get_stored_setting_setting, (common::setting&, std::string), (override));
  MOCK_METHOD(bool, set_stored_setting, (const common::setting&), (override));

  // Мок-методы для bot_info
  MOCK_METHOD(std::optional<common::bot_info>, get_bot_info, (), (override));
  MOCK_METHOD(bool, update_bot_info, (const common::bot_info&), (override));
  MOCK_METHOD(bool, delete_bot_info, (), (override));

  // Мок-методы для users/groups
  MOCK_METHOD(std::vector<common::user>, get_stored_users, (), (override));
  MOCK_METHOD(std::vector<common::group>, get_stored_groups, (), (override));
  MOCK_METHOD(bool, add_user, (const common::user&), (override));
  MOCK_METHOD(bool, add_group, (const common::group&), (override));
  MOCK_METHOD(bool, remove_user, (const std::string&), (override));
  MOCK_METHOD(bool, remove_group, (const std::string&), (override));
  MOCK_METHOD(bool, update_user, (const common::user&), (override));
  MOCK_METHOD(bool, update_group, (const common::group&), (override));

  // Мок-методы для admins
  MOCK_METHOD(std::vector<std::string>, get_admin_ids, (), (override));
  MOCK_METHOD(std::vector<std::string>, get_superadmin_ids, (), (override));
  MOCK_METHOD(bool, add_admin, (const common::user&), (override));
  MOCK_METHOD(bool, add_superadmin, (const common::group&), (override));
  MOCK_METHOD(bool, remove_admin, (const std::string&), (override));
  MOCK_METHOD(bool, remove_superadmin, (const std::string&), (override));

  // Мок-методы для playlists
  MOCK_METHOD(std::vector<common::playlist>, get_group_playlists, (const std::string&, const std::string&), (override));
  MOCK_METHOD(bool, add_playlist, (const common::playlist&), (override));
  MOCK_METHOD(bool, remove_playlist, (const common::playlist&), (override));
  MOCK_METHOD(bool, add_playlist_yandex, (const common::playlist&, const common::playlist::yandex&), (override));
  MOCK_METHOD(bool, remove_playlist_yandex, (const common::playlist&, const common::playlist::yandex&), (override));

  // Делегаты для упрощённого доступа (wrapper-методы)
  // Эти методы вызывают соответствующие mock-методы с корректной сигнатурой

  std::string get_stored_setting(const std::string& key, std::string default_value = std::string()) override {
    return get_stored_setting_string_key(key, std::move(default_value));
  }

  std::string get_stored_setting(common::setting& setting, std::string default_value = std::string()) override {
    return get_stored_setting_setting(setting, std::move(default_value));
  }

  std::vector<common::playlist> get_group_playlists(const std::string& group_id = "",
                                                    const std::string& playlist_id = "") override {
    return get_group_playlists(group_id, playlist_id);
  }
};

}  // namespace ymsummorizer::storage::mocs
