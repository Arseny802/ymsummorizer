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

private:
    std::atomic_bool connected_ = false;
    const storage_types storage_type_;
    const std::string db_name_;
    std::map<std::string, std::string> settings_;

    std::shared_ptr<db::manager_base> manager_pimpl;
  };

} // namespace ymsummorizer::storage