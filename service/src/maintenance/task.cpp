#include "task.h"

#include "pch.h"
#include "ymapi/client.h"

namespace ymsummorizer::service::maintenance {

task::task(storage::db_manager& db) : db_(db) {}

task::~task() = default;

void task::run() noexcept {
  const auto all_playlists = db_.get_group_playlists();

  for (const common::playlist& playlist : all_playlists) {
    const auto all_yandex_info = get_playlists_info(playlist.yandex_users);
  }
}

std::vector<common::playlist> task::get_playlists_info(
    const std::vector<common::playlist::yandex>& yandex_users) noexcept {
  std::vector<common::playlist> result;
  std::string user_token = "";

  for (const auto& yandex_user : yandex_users) {
    ymapi::client ya_client(yandex_user.user_id, user_token);
    ya_client.playlist_get_info(yandex_user.kind);
  }

  return result;
}

std::vector<common::playlist_track> task::get_playlist_tracks(
    const std::vector<common::playlist>& playlists) noexcept {
  std::vector<common::playlist_track> result;
  return result;
}

}  // namespace ymsummorizer::service::maintenance
