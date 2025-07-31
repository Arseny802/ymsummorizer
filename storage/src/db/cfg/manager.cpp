#include "pch.h"

#include <nlohmann/json.hpp>

#include "manager.h"

namespace ymsummorizer::storage::db::cfg {
manager::manager() {
  AUTOTRACE;
}

manager::~manager() {
  AUTOTRACE
}

bool manager::connect(const std::string& cfg_file_name) {
  AUTOTRACE

  cfg_file_path_ = std::filesystem::path(cfg_file_name);
  log()->info("Opening config file '{0}'...", cfg_file_name);
  if (!std::filesystem::exists(cfg_file_path_)) {
    log()->warning("Config file '{0}' not exists! Create basic configuration before use.",
                   cfg_file_name);

    // That's a file, so we can just create another one and fill it with default values
    return true;
  }

  std::ifstream ifs(cfg_file_name);
  data_ = std::make_unique<nlohmann::json>(nlohmann::json::parse(ifs));

  return true;
}

bool manager::flash() {
  AUTOTRACE
  save_data();
  return true;
}

bool manager::create_db() {
  AUTOTRACE

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
  AUTOTRACE
  assert(data_ && "Data is empty!");

  // write prettified JSON to another file
  std::ofstream result_cfg_file(cfg_file_path_);
  result_cfg_file << std::setw(2) << *data_ << std::endl;
  result_cfg_file.close();
}

bool manager::get_stored_setting(common::setting& setting) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  return true;
}

std::optional<std::vector<common::setting>> manager::get_stored_settings() {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  std::vector<common::setting> result;

  return result;
}

bool manager::set_stored_setting(const common::setting& setting) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  return true;
}

std::optional<common::bot_info> manager::get_bot_info() {
  AUTOTRACE
  if (!data_ || !data_->contains("bot_info")) {
    log()->error("Data is empty!");
    return std::nullopt;
  }

  common::bot_info result;
  result.token = (*data_)["bot_info"]["token"];
  return result;
}

bool manager::update_bot_info(const common::bot_info& bot_info) {
  AUTOTRACE
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
  AUTOTRACE
  assert(data_ && "Data is empty!");

  data_->erase("bot_info");
  save_data();

  return false;
}

std::vector<common::user> manager::get_stored_users() {
  AUTOTRACE
  assert(data_ && "Data is empty!");
  assert(data_->contains("users") && !(*data_)["users"].empty() && "No stored users!");

  std::vector<common::user> result;
  for (const auto& user : (*data_)["users"]) {
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
  AUTOTRACE
  assert(data_ && "Data is empty!");
  assert(data_->contains("groups") && !(*data_)["groups"].empty() && "No stored groups!");

  std::vector<common::group> result;
  for (const auto& group : (*data_)["groups"]) {
    common::group current_group;
    current_group.id = group["id"];
    current_group.name = group["name"];
    for (const auto& user : (*data_)["users"]) {
      current_group.user_ids.emplace_back(user.at("id"));
    }

    current_group.playlists = get_group_playlists(current_group.id, "");

    result.emplace_back(std::move(current_group));
  }

  return result;
}

bool manager::add_user(const common::user& user) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  log()->error("add_user not implemented!");
  return false;
}
bool manager::add_group(const common::group& group) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  log()->error("add_group not implemented!");
  return false;
}

bool manager::remove_user(const std::string& user_id) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  log()->error("remove_user not implemented!");
  return false;
}
bool manager::remove_group(const std::string& group_id) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  log()->error("remove_group not implemented!");
  return false;
}

bool manager::update_user(const common::user& user) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  log()->error("update_user not implemented!");
  return false;
}
bool manager::update_group(const common::group& group) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  log()->error("update_group not implemented!");
  return false;
}

std::vector<std::string> manager::get_admin_ids() {
  AUTOTRACE
  assert(data_ && "Data is empty!");
  assert(data_->contains("admins") && !(*data_)["admins"].empty() && "No stored admins!");

  std::vector<std::string> result;
  for (const auto& user : (*data_)["admins"]) {
    result.emplace_back(user);
  }
  return result;
}

std::vector<std::string> manager::get_superadmin_ids() {
  AUTOTRACE
  assert(data_ && "Data is empty!");
  assert(data_->contains("superadmins") && !(*data_)["superadmins"].empty() &&
         "No stored superadmins!");

  std::vector<std::string> result;
  for (const auto& user : (*data_)["superadmins"]) {
    result.emplace_back(user);
  }
  return result;
}

bool manager::add_admin(const common::user& user) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  log()->error("add_admin not implemented!");
  return false;
}
bool manager::add_superadmin(const common::group& group) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  log()->error("add_superadmin not implemented!");
  return false;
}

bool manager::remove_admin(const std::string& user_id) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  log()->error("remove_admin not implemented!");
  return false;
}
bool manager::remove_superadmin(const std::string& group_id) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  log()->error("remove_superadmin not implemented!");
  return false;
}

std::vector<common::playlist> manager::get_group_playlists(const std::string& group_id,
                                                           const std::string& playlist_id) {
  AUTOTRACE
  assert(data_ && "Data is empty!");
  assert(data_->contains("groups") && !(*data_)["groups"].empty() && "No stored groups!");

  std::vector<common::playlist> result;
  for (const auto& current_group : (*data_)["groups"]) {
    std::string current_group_id = current_group.contains("id") ? current_group["id"] : "";
    if (!group_id.empty() && current_group_id != group_id) {
      continue;
    }

    if (!current_group.contains("playlists") || current_group["playlists"].empty()) {
      std::string current_group_name = current_group.contains("name") ? current_group["name"] : "";
      log()->warning("No stored playlists in group '{}' (name: '{}')!",
                     current_group_id,
                     current_group_name);
      if (group_id.empty()) {
        continue;
      }
      return result;
    }

    for (const auto& playlist : current_group["playlists"]) {
      if (!playlist_id.empty() && playlist["id"] != playlist_id) {
        continue;
      }
      common::playlist current_playlist;
      current_playlist.group_id = current_group_id;
      current_playlist.id = playlist["id"];
      current_playlist.name = playlist["name"];

      for (const auto& yandex_user : playlist["yandex"]) {
        common::playlist::yandex current_playlist_yandex;
        if (!yandex_user.contains("user_id") || !yandex_user.contains("kind")) {
          log()->error("Ivalid yandex user in playlist '{}' (name: '{}')!",
                       current_playlist.id,
                       current_playlist.name);
          continue;
        }

        current_playlist_yandex.user_id = yandex_user["user_id"];
        current_playlist_yandex.kind = yandex_user["kind"];
        current_playlist.yandex_users.emplace_back(std::move(current_playlist_yandex));
      }

      result.emplace_back(std::move(current_playlist));
    }
  }

  return result;
}

bool manager::add_playlist(const common::playlist& playlist) {
  AUTOTRACE
  assert(!playlist.group_id.empty() && "playlist.group_id is empty!");
  assert(data_ && "Data is empty!");
  assert(data_->contains("groups") && !(*data_)["groups"].empty() && "No stored groups!");

  for (auto& current_group : (*data_)["groups"]) {
    std::string current_group_id = current_group.contains("id") ? current_group["id"] : "";
    if (current_group_id != playlist.group_id) {
      continue;
    }

    if (!current_group.contains("playlists")) {
      current_group["playlists"] = nlohmann::json::array();
    }
  }

  log()->error("No stored group with id '{}'!", playlist.group_id);
  return false;
}

bool manager::remove_playlist(const common::playlist& playlist) {
  AUTOTRACE
  assert(data_ && "Data is empty!");

  return false;
}

bool manager::add_playlist_yandex(const common::playlist& playlist,
                                  const common::playlist::yandex& playlist_yandex) {
  AUTOTRACE
  assert(!playlist.group_id.empty() && "playlist.group_id is empty!");
  assert(data_ && "Data is empty!");

  return false;
}

bool manager::remove_playlist_yandex(const common::playlist& playlist,
                                     const common::playlist::yandex& playlist_yandex) {
  AUTOTRACE
  assert(!playlist.group_id.empty() && "playlist.group_id is empty!");
  assert(data_ && "Data is empty!");

  return false;
}
}  // namespace ymsummorizer::storage::db::cfg
