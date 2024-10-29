#pragma once
#include "../manager_base.h"
#include "common/common.hpp"

#include <sqlite3.h>

namespace ymsummorizer::storage::db::sqlite {
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
    std::string get_db_version();
    bool execute_query(const char* sql_query);

    template<typename TArg>
    static void prapare_statement_arg(sqlite3_stmt* stmt, TArg argument, int index);
    template<typename... TArgs>
    sqlite3_stmt* prapare_statement(const char* sql_query, TArgs&&... arguments);

    sqlite3* db_handler_;
  };
} // namespace ymsummorizer::storage::db::sqlite
#include "manager.hpp"
