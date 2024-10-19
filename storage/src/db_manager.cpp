#include "pch.h"
#include "storage/db_manager.h"
#include "db/fabric.h"

namespace gazeta::storage {

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

  /// source_types (data reciever types)
  std::vector<common::source_type> db_manager::get_source_types() {
    AUTOLOG_ST
    auto sources = manager_pimpl->get_source_types();
    if (!sources.has_value()) {
      log()->error("Get source types failed!");
      return {};
    }
    return sources.value();
  }

  /// sources (data URLs)
  bool db_manager::get_info_source(common::source& info_source) {
    AUTOLOG_ST
    return manager_pimpl->get_info_source(info_source);
  }

  std::vector<common::source> db_manager::get_info_sources() {
    AUTOLOG_ST
    auto info_sources = manager_pimpl->get_info_sources();
    if (!info_sources.has_value()) {
      log()->error("Get info sources failed!");
      return {};
    }
    return info_sources.value();
  }

  bool db_manager::set_info_source(const common::source& info_source) {
    AUTOLOG_ST
    return manager_pimpl->set_info_source(info_source);
  }

  /// articles (concrete data)
  bool db_manager::get_article(common::article& article) {
    AUTOLOG_ST
    return manager_pimpl->get_article(article);
  }

  std::vector<common::article> db_manager::get_articles(size_t limit) {
    AUTOLOG_ST
    auto articles = manager_pimpl->get_articles(limit);
    if (!articles.has_value()) {
      log()->error("Get articles failed!");
      return {};
    }
    return articles.value();
  }

  std::vector<common::article> db_manager::get_articles(int timestamp_begin) {
    AUTOLOG_ST
    auto articles = manager_pimpl->get_articles(timestamp_begin);
    if (!articles.has_value()) {
      log()->error("Get articles failed!");
      return {};
    }
    return articles.value();
  }

  std::vector<common::article> db_manager::get_articles(int timestamp_begin, int timestamp_end) {
    AUTOLOG_ST
    auto articles = manager_pimpl->get_articles(timestamp_begin, timestamp_end);
    if (!articles.has_value()) {
      log()->error("Get articles failed!");
      return {};
    }
    return articles.value();
  }

  std::vector<common::article> db_manager::get_articles_by_source(int source_id, size_t limit) {
    AUTOLOG_ST
    auto articles = manager_pimpl->get_articles_by_source(source_id, limit);
    if (!articles.has_value()) {
      log()->error("Get articles failed!");
      return {};
    }
    return articles.value();
  }

  std::vector<common::article> db_manager::get_articles_by_source(int source_id, int timestamp_begin) {
    AUTOLOG_ST
    auto articles = manager_pimpl->get_articles_by_source(source_id, timestamp_begin);
    if (!articles.has_value()) {
      log()->error("Get articles failed!");
      return {};
    }
    return articles.value();
  }

  std::vector<common::article> db_manager::get_articles_by_source(int source_id,
                                                                  int timestamp_begin,
                                                                  int timestamp_end) {
    AUTOLOG_ST
    auto articles = manager_pimpl->get_articles_by_source(source_id, timestamp_begin, timestamp_end);
    if (!articles.has_value()) {
      log()->error("Get articles failed!");
      return {};
    }
    return articles.value();
  }

  bool db_manager::set_article(const common::article& article) {
    AUTOLOG_ST
    return manager_pimpl->set_article(article);
  }

  bool db_manager::set_articles(const std::vector<common::article>& articles) {
    AUTOLOG_ST
    return manager_pimpl->set_articles(articles);
  }
} // namespace gazeta::storage
