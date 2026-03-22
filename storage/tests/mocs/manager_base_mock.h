#pragma once
#include <gmock/gmock.h>
#include "../../src/db/manager_base.h"

namespace ymsummorizer::storage::db {

class manager_base_mock : public manager_base {
public:
    MOCK_METHOD(bool, connect, (const std::string& db_name), (override));
    MOCK_METHOD(bool, flash, (), (override));
    MOCK_METHOD(bool, create_db, (), (override));

    MOCK_METHOD(bool, get_stored_setting, (common::setting& key), (override));
    MOCK_METHOD((std::optional<std::vector<common::setting>>), get_stored_settings, (), (override));
    MOCK_METHOD(bool, set_stored_setting, (const common::setting& setting), (override));

    MOCK_METHOD((std::optional<common::bot_info>), get_bot_info, (), (override));
    MOCK_METHOD(bool, update_bot_info, (const common::bot_info& bot_info), (override));
    MOCK_METHOD(bool, delete_bot_info, (), (override));

    MOCK_METHOD(std::vector<common::user>, get_stored_users, (), (override));
    MOCK_METHOD(std::vector<common::group>, get_stored_groups, (), (override));

    MOCK_METHOD(bool, add_user, (const common::user& user), (override));
    MOCK_METHOD(bool, add_group, (const common::group& group), (override));

    MOCK_METHOD(bool, remove_user, (const std::string& user_id), (override));
    MOCK_METHOD(bool, remove_group, (const std::string& group_id), (override));

    MOCK_METHOD(bool, update_user, (const common::user& user), (override));
    MOCK_METHOD(bool, update_group, (const common::group& group), (override));

    MOCK_METHOD(std::vector<std::string>, get_admin_ids, (), (override));
    MOCK_METHOD(std::vector<std::string>, get_superadmin_ids, (), (override));

    MOCK_METHOD(bool, add_admin, (const common::user& user), (override));
    MOCK_METHOD(bool, add_superadmin, (const common::group& group), (override));

    MOCK_METHOD(bool, remove_admin, (const std::string& user_id), (override));
    MOCK_METHOD(bool, remove_superadmin, (const std::string& group_id), (override));

    MOCK_METHOD(std::vector<common::playlist>, get_group_playlists, (const std::string& group_id, const std::string& playlist_id), (override));

    MOCK_METHOD(bool, add_playlist, (const common::playlist& playlist), (override));
    MOCK_METHOD(bool, remove_playlist, (const common::playlist& playlist), (override));

    MOCK_METHOD(bool, add_playlist_yandex, (const common::playlist& playlist, const common::playlist::yandex& playlist_yandex), (override));
    MOCK_METHOD(bool, remove_playlist_yandex, (const common::playlist& playlist, const common::playlist::yandex& playlist_yandex), (override));
};

} // namespace ymsummorizer::storage::db
