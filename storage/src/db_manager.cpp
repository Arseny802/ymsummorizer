#include "pch.h"
#include "storage/db_manager.h"
#include "db/fabric.h"

namespace ymsummorizer::storage {

  db_manager::db_manager(storage_types storage_type, std::string db_name):
      storage_type_(storage_type),
      db_name_(std::move(db_name)) {
    AUTOLOG_ST;
  }

  db_manager::~db_manager() {
    AUTOLOG_ST
  }

  bool db_manager::connect() {
    AUTOLOG_ST

    if (connected_) {
      return true;
    }

    manager_pimpl = db::fabric::create(storage_type_);
    if (!manager_pimpl->connect(db_name_)) {
      return false;
    }

    if (!manager_pimpl->create_db()) {
      return false;
    }

    if (const auto settings = manager_pimpl->get_stored_settings(); settings.has_value()) {
      for (const auto& [key, value]: settings.value()) {
        log()->info("Got setting ['{}' => '{}'].", key, value);
        settings_[key] = value;
      }
    } else {
      log()->warning("No stored settings found in DB.");
    }

    connected_ = true;
    return true;
  }

  /// settings
  std::string db_manager::get_stored_setting(common::setting& setting, std::string default_value) {
    AUTOLOG_ST

    if (const auto it = settings_.find(setting.key); it != settings_.end()) {
      return it->second;
    }

    setting.value = default_value;
    if (setting.key.empty()) {
      log()->warning("get_stored_setting error: setting.key is empty.");
    } else if (manager_pimpl->get_stored_setting(setting)) {
      return setting.value;
    }

    log()->warning("Get setting failed, key: '{}'. default_value: '{}'.", setting.key, default_value);
    return default_value;
  }

  std::string db_manager::get_stored_setting(const std::string& key, std::string default_value) {
    AUTOLOG_ST

    common::setting setting;
    setting.key = key;

    if (manager_pimpl->get_stored_setting(setting)) {
      return setting.value;
    }

    log()->warning("Get setting failed, key: '{}'. default_value: '{}'.", key, default_value);
    return default_value;
  }

  bool db_manager::set_stored_setting(const common::setting& setting) {
    AUTOLOG_ST
    return manager_pimpl->set_stored_setting(setting);
  }

  std::vector<common::user> db_manager::get_stored_users() {
    AUTOLOG_ST
    return manager_pimpl->get_stored_users();
  }
  std::vector<common::group> db_manager::get_stored_groups() {
    AUTOLOG_ST
    return manager_pimpl->get_stored_groups();
  }

  std::optional<common::bot_info> db_manager::get_bot_info() {
    AUTOLOG_ST
    return manager_pimpl->get_bot_info();
  }

  bool db_manager::update_bot_info(const common::bot_info& bot_info) {
    AUTOLOG_ST
    return manager_pimpl->update_bot_info(bot_info);
  }

  bool db_manager::delete_bot_info() {
    AUTOLOG_ST
    return manager_pimpl->delete_bot_info();
  }

  bool db_manager::add_user(const common::user& user) {
    AUTOLOG_ST
    return manager_pimpl->add_user(user);
  }
  bool db_manager::add_group(const common::group& group) {
    AUTOLOG_ST
    return manager_pimpl->add_group(group);
  }

  bool db_manager::remove_user(const std::string& user_id) {
    AUTOLOG_ST
    return manager_pimpl->remove_user(user_id);
  }
  bool db_manager::remove_group(const std::string& group_id) {
    AUTOLOG_ST
    return manager_pimpl->remove_group(group_id);
  }

  bool db_manager::update_user(const common::user& user) {
    AUTOLOG_ST
    return manager_pimpl->update_user(user);
  }
  bool db_manager::update_group(const common::group& group) {
    AUTOLOG_ST
    return manager_pimpl->update_group(group);
  }

  std::vector<std::string> db_manager::get_admin_ids() {
    AUTOLOG_ST
    return manager_pimpl->get_admin_ids();
  }
  std::vector<std::string> db_manager::get_superadmin_ids() {
    AUTOLOG_ST
    return manager_pimpl->get_superadmin_ids();
  }

  bool db_manager::add_admin(const common::user& user) {
    AUTOLOG_ST
    return manager_pimpl->add_admin(user);
  }
  bool db_manager::add_superadmin(const common::group& group) {
    AUTOLOG_ST
    return manager_pimpl->add_superadmin(group);
  }

  bool db_manager::remove_admin(const std::string& user_id) {
    AUTOLOG_ST
    return manager_pimpl->remove_admin(user_id);
  }
  bool db_manager::remove_superadmin(const std::string& group_id) {
    AUTOLOG_ST
    return manager_pimpl->remove_superadmin(group_id);
  }
} // namespace ymsummorizer::storage
