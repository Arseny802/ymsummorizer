#pragma once
#include <string>

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

    bool connect();

    /// settings
    std::string get_stored_setting(const std::string& key, std::string default_value = std::string());
    std::string get_stored_setting(common::setting& setting, std::string default_value = std::string());
    bool set_stored_setting(const common::setting& setting);

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

private:
    std::atomic_bool connected_ = false;
    const storage_types storage_type_;
    const std::string db_name_;
    std::map<std::string, std::string> settings_;

    std::shared_ptr<db::manager_base> manager_pimpl;
  };

} // namespace ymsummorizer::storage