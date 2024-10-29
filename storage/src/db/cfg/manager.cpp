#include "pch.h"
#include "manager.h"
#include <nlohmann/json.hpp>

namespace ymsummorizer::storage::db::cfg {
  manager::manager() {
    AUTOLOG_ST;
  }

  manager::~manager() {
    AUTOLOG_ST
  }

  bool manager::connect(const std::string& cfg_file_name) {
    AUTOLOG_ST

    cfg_file_path_ = std::filesystem::path(cfg_file_name);
    log()->info("Opening config file '{0}'...", cfg_file_name);
    if (!std::filesystem::exists(cfg_file_path_)) {
      log()->warning("Config file '{0}' not exists! Create basic configuration before use.", cfg_file_name);

      // That's a file, so we can just create another one and fill it with default values
      return true;
    }

    std::ifstream ifs(cfg_file_name);
    data_ = std::make_unique<nlohmann::json>(nlohmann::json::parse(ifs));

    return true;
  }

  bool manager::create_db() {
    AUTOLOG_ST

    if (!data_) {
      log()->warning("Data is empty! Creating base configuration...");
      data_ = std::make_unique<nlohmann::json>();
    }

    try {
      if (!data_->contains("bot_info")) {
        (*data_)["bot_info"] = nlohmann::json::object();
        (*data_)["bot_info"]["token"] = "INSERT_YOUR_BOT_TOKEN_HERE";
      }

      if (!data_->contains("users")) {
        (*data_)["users"] = nlohmann::json::array();
      }
      if ((*data_)["users"].size() == 0) {
        nlohmann::json user;
        user["id"] = boost::uuids::to_string(boost::uuids::random_generator()());
        user["name"] = "User01_name";
        user["login_tg"] = "User01_login_tg";
        user["login_yandex"] = "User01_login_yandex";
        user["token"] = "User01_yandex_token";
        (*data_)["users"].emplace_back(std::move(user));
      }

      if ((*data_)["users"].size() == 1) {
        nlohmann::json user;
        user["id"] = boost::uuids::to_string(boost::uuids::random_generator()());
        user["name"] = "User02_name";
        user["login_tg"] = "User02_login_tg";
        user["login_yandex"] = "User02_login_yandex";
        user["token"] = "User02_yandex_token";
        (*data_)["users"].emplace_back(std::move(user));
      }

      if (!data_->contains("groups")) {
        (*data_)["groups"] = nlohmann::json::array();
      }
      if ((*data_)["groups"].size() == 0) {
        nlohmann::json group;
        group["id"] = boost::uuids::to_string(boost::uuids::random_generator()());
        group["name"] = "DefaultGoup";
        group["users"] = nlohmann::json::array();
        group["users"].emplace_back((*data_)["users"][0]["id"]);
        group["users"].emplace_back((*data_)["users"][1]["id"]);
        (*data_)["groups"].emplace_back(std::move(group));
      }

      if (!data_->contains("admins")) {
        (*data_)["admins"] = nlohmann::json::array();
        (*data_)["admins"].emplace_back((*data_)["users"][0]["id"]);
      }

      if (!data_->contains("superadmins")) {
        (*data_)["superadmins"] = nlohmann::json::array();
        (*data_)["superadmins"].emplace_back((*data_)["users"][0]["id"]);
      }

      save_data();
      return true;
    } catch (const std::exception& e) {
      log()->error("Error: {0}", e.what());
    }
    return false;
  }
  void manager::save_data() {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return;
    }

    // write prettified JSON to another file
    std::ofstream result_cfg_file(cfg_file_path_);
    result_cfg_file << std::setw(2) << *data_ << std::endl;
    result_cfg_file.close();
  }

  bool manager::get_stored_setting(common::setting& setting) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    return true;
  }

  std::optional<std::vector<common::setting>> manager::get_stored_settings() {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return std::nullopt;
    }

    std::vector<common::setting> result;

    return result;
  }

  bool manager::set_stored_setting(const common::setting& setting) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    return true;
  }

  std::optional<common::bot_info> manager::get_bot_info() {
    AUTOLOG_ST
    if (!data_ || !data_->contains("bot_info")) {
      log()->error("Data is empty!");
      return std::nullopt;
    }

    common::bot_info result;
    result.token = (*data_)["bot_info"]["token"];
    return result;
  }

  bool manager::update_bot_info(const common::bot_info& bot_info) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    if (!data_->contains("bot_info")) {
      log()->warning("Bot info is empty!");
      (*data_)["bot_info"] = nlohmann::json::object();
    }

    (*data_)["bot_info"]["token"] = bot_info.token;
    save_data();

    return true;
  }

  bool manager::delete_bot_info() {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return true;
    }

    data_->erase("bot_info");
    save_data();

    return false;
  }

  std::vector<common::user> manager::get_stored_users() {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return {};
    }
    if (!data_->contains("users") || (*data_)["users"].empty()) {
      log()->error("No stored users!");
      return {};
    }

    std::vector<common::user> result;
    for (const auto& user: (*data_)["users"]) {
      common::user current_user;
      current_user.id = user["id"];
      current_user.name = user["name"];
      current_user.login_tg = user["login_tg"];
      current_user.login_yandex = user["login_yandex"];
      current_user.token = user["token"];
      result.emplace_back(std::move(current_user));
    }
    return result;
  }

  std::vector<common::group> manager::get_stored_groups() {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return {};
    }
    if (!data_->contains("groups") || (*data_)["groups"].empty()) {
      log()->error("No stored users!");
      return {};
    }

    std::vector<common::group> result;
    for (const auto& group: (*data_)["groups"]) {
      common::group current_group;
      current_group.id = group["id"];
      current_group.name = group["name"];
      for (const auto& user: (*data_)["users"]) {
        current_group.user_ids.emplace_back(user.at("id"));
      }
      result.emplace_back(std::move(current_group));
    }

    return result;
  }

  bool manager::add_user(const common::user& user) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    log()->error("add_user not implemented!");
    return false;
  }
  bool manager::add_group(const common::group& group) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    log()->error("add_group not implemented!");
    return false;
  }

  bool manager::remove_user(const std::string& user_id) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    log()->error("remove_user not implemented!");
    return false;
  }
  bool manager::remove_group(const std::string& group_id) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    log()->error("remove_group not implemented!");
    return false;
  }

  bool manager::update_user(const common::user& user) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    log()->error("update_user not implemented!");
    return false;
  }
  bool manager::update_group(const common::group& group) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    log()->error("update_group not implemented!");
    return false;
  }

  std::vector<std::string> manager::get_admin_ids() {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return {};
    }
    if (!data_->contains("admins") || (*data_)["admins"].empty()) {
      log()->error("No stored users!");
      return {};
    }

    std::vector<std::string> result;
    for (const auto& user: (*data_)["admins"]) {
      result.emplace_back(user);
    }
    return result;
  }

  std::vector<std::string> manager::get_superadmin_ids() {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return {};
    }
    if (!data_->contains("superadmins") || (*data_)["superadmins"].empty()) {
      log()->error("No stored users!");
      return {};
    }

    std::vector<std::string> result;
    for (const auto& user: (*data_)["superadmins"]) {
      result.emplace_back(user);
    }
    return result;
  }

  bool manager::add_admin(const common::user& user) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    log()->error("add_admin not implemented!");
    return false;
  }
  bool manager::add_superadmin(const common::group& group) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    log()->error("add_superadmin not implemented!");
    return false;
  }

  bool manager::remove_admin(const std::string& user_id) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    log()->error("remove_admin not implemented!");
    return false;
  }
  bool manager::remove_superadmin(const std::string& group_id) {
    AUTOLOG_ST
    if (!data_) {
      log()->error("Data is empty!");
      return false;
    }

    log()->error("remove_superadmin not implemented!");
    return false;
  }
} // namespace ymsummorizer::storage::db::cfg
