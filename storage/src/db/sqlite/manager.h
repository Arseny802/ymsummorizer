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

    /// source_types (data reciever types)
    std::optional<std::vector<common::source_type>> get_source_types();

    /// sources (data URLs)
    bool get_info_source(common::source& info_source);
    std::optional<std::vector<common::source>> get_info_sources();
    bool set_info_source(const common::source& info_source);

    /// articles (concrete data)
    bool get_article(common::article& article);
    std::optional<std::vector<common::article>> get_articles(size_t limit = 100);
    std::optional<std::vector<common::article>> get_articles(int timestamp_begin);
    std::optional<std::vector<common::article>> get_articles(int timestamp_begin, int timestamp_end);
    std::optional<std::vector<common::article>> get_articles_by_source(int source_id, size_t limit = 100);
    std::optional<std::vector<common::article>> get_articles_by_source(int source_id, int timestamp_begin);
    std::optional<std::vector<common::article>> get_articles_by_source(int source_id,
                                                                       int timestamp_begin,
                                                                       int timestamp_end);
    bool set_article(const common::article& article);
    bool set_articles(const std::vector<common::article>& articles);

private:
    std::string get_db_version();
    bool execute_query(const char* sql_query);

    std::optional<std::vector<common::article>> get_article_data(const char* sql_query, sqlite3_stmt* stmt);

    template<typename TArg>
    static void prapare_statement_arg(sqlite3_stmt* stmt, TArg argument, int index);
    template<typename... TArgs>
    sqlite3_stmt* prapare_statement(const char* sql_query, TArgs&&... arguments);

    sqlite3* db_handler_;
  };
} // namespace ymsummorizer::storage::db::sqlite
#include "manager.hpp"
