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
    virtual bool create_db() = 0;

    /// settings
    virtual bool get_stored_setting(common::setting& key) = 0;
    virtual std::optional<std::vector<common::setting>> get_stored_settings() = 0;
    virtual bool set_stored_setting(const common::setting& setting) = 0;

    /// source_types (data reciever types)
    virtual std::optional<std::vector<common::source_type>> get_source_types() = 0;

    /// sources (data URLs)
    virtual bool get_info_source(common::source& info_source) = 0;
    virtual std::optional<std::vector<common::source>> get_info_sources() = 0;
    virtual bool set_info_source(const common::source& info_source) = 0;

    /// articles (concrete data)
    virtual bool get_article(common::article& article) = 0;
    virtual std::optional<std::vector<common::article>> get_articles(size_t limit = 100) = 0;
    virtual std::optional<std::vector<common::article>> get_articles(int timestamp_begin) = 0;
    virtual std::optional<std::vector<common::article>> get_articles(int timestamp_begin, int timestamp_end) = 0;
    virtual std::optional<std::vector<common::article>> get_articles_by_source(int source_id, size_t limit = 100) = 0;
    virtual std::optional<std::vector<common::article>> get_articles_by_source(int source_id, int timestamp_begin) = 0;
    virtual std::optional<std::vector<common::article>> get_articles_by_source(int source_id,
                                                                               int timestamp_begin,
                                                                               int timestamp_end) = 0;
    virtual bool set_article(const common::article& article) = 0;
    virtual bool set_articles(const std::vector<common::article>& articles) = 0;

protected:
    std::string get_sql_file(const std::string& file_name) const;

    const std::string_view db_version_setting_key = "db.version";
  };
  using manager_base_ptr = std::shared_ptr<manager_base>;

} // namespace ymsummorizer::storage::db
