#pragma once
#include "common/common.hpp"
#include <memory>
#include <string>

namespace ymsummorizer::storage::db {
  class manager_base {
public:
    manager_base();
    virtual ~manager_base();

    virtual bool connect(const std::string& db_name) = 0;
    virtual bool flash() = 0;
    virtual bool create_db() = 0;

    /// settings
    virtual bool get_stored_setting(common::setting& key) = 0;
    virtual std::optional<std::vector<common::setting>> get_stored_settings() = 0;
    virtual bool set_stored_setting(const common::setting& setting) = 0;

    virtual std::optional<common::bot_info> get_bot_info() = 0;
    virtual bool update_bot_info(const common::bot_info& bot_info) = 0;
    virtual bool delete_bot_info() = 0;

    virtual std::vector<common::user> get_stored_users() = 0;
    virtual std::vector<common::group> get_stored_groups() = 0;

    virtual bool add_user(const common::user& user) = 0;
    virtual bool add_group(const common::group& group) = 0;

    virtual bool remove_user(const std::string& user_id) = 0;
    virtual bool remove_group(const std::string& group_id) = 0;

    virtual bool update_user(const common::user& user) = 0;
    virtual bool update_group(const common::group& group) = 0;

    virtual std::vector<std::string> get_admin_ids() = 0;
    virtual std::vector<std::string> get_superadmin_ids() = 0;

    virtual bool add_admin(const common::user& user) = 0;
    virtual bool add_superadmin(const common::group& group) = 0;

    virtual bool remove_admin(const std::string& user_id) = 0;
    virtual bool remove_superadmin(const std::string& group_id) = 0;

    virtual std::vector<common::playlist> get_group_playlists(const std::string& group_id = "",
                                                              const std::string& playlist_id = "") = 0;

    virtual bool add_playlist(const common::playlist& playlist) = 0;
    virtual bool remove_playlist(const common::playlist& playlist) = 0;

    virtual bool add_playlist_yandex(const common::playlist& playlist,
                                     const common::playlist::yandex& playlist_yandex) = 0;
    virtual bool remove_playlist_yandex(const common::playlist& playlist,
                                        const common::playlist::yandex& playlist_yandex) = 0;

protected:
    std::string get_sql_file(const std::string& file_name) const;

    const std::string_view db_version_setting_key = "db.version";
  };
  using manager_base_ptr = std::shared_ptr<manager_base>;

} // namespace ymsummorizer::storage::db
