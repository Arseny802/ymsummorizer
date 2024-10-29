#pragma once
#include "../manager_base.h"
#include "common/common.hpp"

#include <filesystem>
#include <memory>
#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

namespace ymsummorizer::storage::db::cfg {
  class manager: public manager_base {
public:
    manager();
    ~manager() override;

    bool connect(const std::string& db_name) override;
    bool create_db() override;

    /// settings
    bool get_stored_setting(common::setting& key);
    std::optional<std::vector<common::setting>> get_stored_settings();
    bool set_stored_setting(const common::setting& setting);

    std::optional<common::bot_info> get_bot_info();
    bool update_bot_info(const common::bot_info& bot_info);
    bool delete_bot_info();

    std::vector<common::user> get_stored_users() override;
    std::vector<common::group> get_stored_groups() override;

    bool add_user(const common::user& user) override;
    bool add_group(const common::group& group) override;

    bool remove_user(const std::string& user_id) override;
    bool remove_group(const std::string& group_id) override;

    bool update_user(const common::user& user) override;
    bool update_group(const common::group& group) override;

    std::vector<std::string> get_admin_ids() override;
    std::vector<std::string> get_superadmin_ids() override;

    bool add_admin(const common::user& user) override;
    bool add_superadmin(const common::group& group) override;

    bool remove_admin(const std::string& user_id) override;
    bool remove_superadmin(const std::string& group_id) override;

private:
    void save_data();

    std::unique_ptr<nlohmann::json> data_;
    std::filesystem::path cfg_file_path_;
  };
} // namespace ymsummorizer::storage::db::cfg
