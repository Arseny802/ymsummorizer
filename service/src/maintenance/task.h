#pragma once
#include "common/common.hpp"

namespace ymsummorizer::service::maintenance {
class task final {
 public:
  task(storage::db_manager& db);
  ~task();
  void run() noexcept;

 private:
  std::vector<common::playlist> get_playlists_info(
      const std::vector<common::playlist::yandex>& yandex_users) noexcept;
  std::vector<common::playlist_track> get_playlist_tracks(
      const std::vector<common::playlist>& playlists) noexcept;

  storage::db_manager& db_;
};
}  // namespace ymsummorizer::service::maintenance
