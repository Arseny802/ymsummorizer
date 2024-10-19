#pragma once
#include "../manager_base.h"
#include "common/common.hpp"

#include <sqlite3.h>

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

private:
  };
} // namespace ymsummorizer::storage::db::cfg
