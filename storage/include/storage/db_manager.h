#pragma once
#include <optional>
#include <string>
#include <vector>

#include "common/common.hpp"
#include "types.h"

namespace ymsummorizer::storage {
  namespace db {
    class manager_base;
  } // namespace db

  class db_manager {
public:
    db_manager(storage_types storage_type, std::string db_name);
    ~db_manager();

    db_manager(const db_manager&) = delete;
    db_manager& operator=(const db_manager&) = delete;

    db_manager(db_manager&& other);
    db_manager& operator=(db_manager&& other) = delete;

    bool connect();
    bool flash();

    /// settings
    std::string get_stored_setting(const std::string& key, std::string default_value = std::string());
    std::string get_stored_setting(common::setting& setting, std::string default_value = std::string());
    bool set_stored_setting(const common::setting& setting);

    std::optional<common::bot_info> get_bot_info();
    bool update_bot_info(const common::bot_info& bot_info);
    bool delete_bot_info();

    std::vector<common::user> get_stored_users();
    std::vector<common::group> get_stored_groups();

    bool add_user(const common::user& user);
    bool add_group(const common::group& group);

    bool remove_user(const std::string& user_id);
    bool remove_group(const std::string& group_id);

    bool update_user(const common::user& user);
    bool update_group(const common::group& group);

    std::vector<std::string> get_admin_ids();
    std::vector<std::string> get_superadmin_ids();

    bool add_admin(const common::user& user);
    bool add_superadmin(const common::group& group);

    bool remove_admin(const std::string& user_id);
    bool remove_superadmin(const std::string& group_id);

    std::vector<common::playlist> get_group_playlists(const std::string& group_id = "",
                                                      const std::string& playlist_id = "");

    bool add_playlist(const common::playlist& playlist);
    bool remove_playlist(const common::playlist& playlist);

    bool add_playlist_yandex(const common::playlist& playlist, const common::playlist::yandex& playlist_yandex);
    bool remove_playlist_yandex(const common::playlist& playlist, const common::playlist::yandex& playlist_yandex);

private:
    std::atomic_bool connected_ = false;
    const storage_types storage_type_;
    const std::string db_name_;
    std::map<std::string, std::string> settings_;

    std::shared_ptr<db::manager_base> manager_pimpl;
  };

} // namespace ymsummorizer::storage