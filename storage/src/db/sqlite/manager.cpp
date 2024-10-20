#include "pch.h"
#include "manager.h"

#include <sqlite3.h>

namespace ymsummorizer::storage::db::sqlite {
  manager::manager() {
    AUTOLOG_ST;
  }

  manager::~manager() {
    AUTOLOG_ST
    sqlite3_close(db_handler_);
  }

  bool manager::connect(const std::string& db_name) {
    AUTOLOG_ST
    log()->info("Connecting to {0}...", db_name);

    int exit = sqlite3_open(db_name.c_str(), &db_handler_);

    if (exit) {
      log()->error("Error open DB: {0}", sqlite3_errmsg(db_handler_));
      return false;
    } else {
      log()->info("Opened Database '{0}' Successfully!", db_name);
    }

    return true;
  }

  bool manager::create_db() {
    AUTOLOG_ST

    try {
      std::string create_db = get_sql_file("sqlite/create_db.sql");

      char* messaggeError;
      int exit = sqlite3_exec(db_handler_, create_db.c_str(), NULL, 0, &messaggeError);

      if (exit != SQLITE_OK) {
        log()->error("SQL error: {0}", messaggeError);
        sqlite3_free(messaggeError);
      } else {
        log()->info("Database created Successfully!");
        set_stored_setting({db_version_setting_key.data(), get_db_version()});
        return true;
      }
    } catch (const std::exception& e) {
      log()->error("Error: {0}", e.what());
    }
    return false;
  }

  std::string manager::get_db_version() {
    AUTOLOG_ST
    const char* get_version_sql = "SELECT SQLITE_VERSION();";
    sqlite3_stmt* stmt = prapare_statement(get_version_sql);
    if (!stmt || !execute_query(get_version_sql)) {
      sqlite3_finalize(stmt);
      return {};
    }

    std::string version_sqlite = (char*)sqlite3_column_text(stmt, 0);
    sqlite3_finalize(stmt);
    return version_sqlite;
  }

  bool manager::execute_query(const char* sql_query) {
    static size_t query_id = 1;
    const size_t current_query_id = query_id++;

    log()->debug("execute_query[{0}] started....", current_query_id);
    log()->trace("query[{0}]: {1}", current_query_id, sql_query);
    try {
      char* messaggeError;
      int exit = sqlite3_exec(db_handler_, sql_query, NULL, 0, &messaggeError);

      if (exit == SQLITE_OK) {
        log()->debug("execute_query[{0}] succeed.", current_query_id);
        return true;
      }

      log()->error("execute_query[{0}] failed, exit code {1}: {2}", current_query_id, exit, messaggeError);
      sqlite3_free(messaggeError);
    } catch (const std::exception& e) {
      log()->error("execute_query[{0}] failed: {1}", current_query_id, e.what());
    }
    return false;
  }

  bool manager::get_stored_setting(common::setting& setting) {
    AUTOLOG_ST
    const char* sql_query = "SELECT `value` FROM `settings` WHERE `key`=?";
    sqlite3_stmt* stmt = prapare_statement(sql_query, setting.key);
    if (!stmt || !execute_query(sql_query)) {
      sqlite3_finalize(stmt);
      return false;
    }

    setting.value = (char*)sqlite3_column_text(stmt, 0);

    sqlite3_finalize(stmt);
    return true;
  }

  std::optional<std::vector<common::setting>> manager::get_stored_settings() {
    AUTOLOG_ST
    const char* sql_query = "SELECT `key`, `value` FROM `settings`";
    sqlite3_stmt* stmt = prapare_statement(sql_query);
    if (!stmt || !execute_query(sql_query)) {
      sqlite3_finalize(stmt);
      return std::nullopt;
    }

    std::vector<common::setting> result;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      common::setting item;
      item.key = (char*)sqlite3_column_text(stmt, 0);
      item.value = (char*)sqlite3_column_text(stmt, 1);
      result.emplace_back(item);
    }

    sqlite3_finalize(stmt);
    return result;
  }

  bool manager::set_stored_setting(const common::setting& setting) {
    AUTOLOG_ST
    const char* sql_query = "INSERT OR REPLACE INTO `settings`(`key`, `value`) VALUES (?,?)";
    sqlite3_stmt* stmt = prapare_statement(sql_query, setting.key, setting.value);
    if (!stmt || !execute_query(sql_query)) {
      sqlite3_finalize(stmt);
      return false;
    }

    int last_id = sqlite3_last_insert_rowid(db_handler_);
    log()->debug("set_stored_setting succeed. id = {0}", last_id);

    sqlite3_finalize(stmt);
    return true;
  }

  std::vector<common::user> manager::get_stored_users() {
    AUTOLOG_ST
    log()->error("get_stored_users not implemented!");
    return {};
  }
  std::vector<common::group> manager::get_stored_groups() {
    AUTOLOG_ST
    log()->error("get_stored_groups not implemented!");
    return {};
  }

  bool manager::add_user(const common::user& user) {
    AUTOLOG_ST
    log()->error("add_user not implemented!");
    return false;
  }
  bool manager::add_group(const common::group& group) {
    AUTOLOG_ST
    log()->error("add_group not implemented!");
    return false;
  }

  bool manager::remove_user(const std::string& user_id) {
    AUTOLOG_ST
    log()->error("remove_user not implemented!");
    return false;
  }
  bool manager::remove_group(const std::string& group_id) {
    AUTOLOG_ST
    log()->error("remove_group not implemented!");
    return false;
  }

  bool manager::update_user(const common::user& user) {
    AUTOLOG_ST
    log()->error("update_user not implemented!");
    return false;
  }
  bool manager::update_group(const common::group& group) {
    AUTOLOG_ST
    log()->error("update_group not implemented!");
    return false;
  }

  std::vector<std::string> manager::get_admin_ids() {
    AUTOLOG_ST
    log()->error("get_admin_ids not implemented!");
    return {};
  }
  std::vector<std::string> manager::get_superadmin_ids() {
    AUTOLOG_ST
    log()->error("get_superadmin_ids not implemented!");
    return {};
  }

  bool manager::add_admin(const common::user& user) {
    AUTOLOG_ST
    log()->error("add_admin not implemented!");
    return false;
  }
  bool manager::add_superadmin(const common::group& group) {
    AUTOLOG_ST
    log()->error("add_superadmin not implemented!");
    return false;
  }

  bool manager::remove_admin(const std::string& user_id) {
    AUTOLOG_ST
    log()->error("remove_admin not implemented!");
    return false;
  }
  bool manager::remove_superadmin(const std::string& group_id) {
    AUTOLOG_ST
    log()->error("remove_superadmin not implemented!");
    return false;
  }
} // namespace ymsummorizer::storage::db::sqlite
