#pragma once
#include <gmock/gmock.h>

#include "../include/storage/types.h"
#include "../src/db/manager_base.h"
#include "common/common.hpp"

namespace ymsummorizer::storage {

class db_manager_mock final : public db::manager_base {
 public:
  db_manager_mock() = default;
  ~db_manager_mock() = default;

  // Мок-методы для connect/flash
  MOCK_METHOD(bool, connect, (const std::string&), (override));
  MOCK_METHOD(bool, flash, (), (override));
  MOCK_METHOD(bool, create_db, (), (override));

  // Мок-методы для settings
  MOCK_METHOD(bool, get_stored_setting, (common::setting&), (override));
  MOCK_METHOD(std::optional<std::vector<common::setting>>, get_stored_settings, (), (override));
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
};

}  // namespace ymsummorizer::storage
