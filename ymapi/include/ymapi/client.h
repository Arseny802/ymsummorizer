#pragma once
#include "playlist.h"
#include <string>
#include <vector>

namespace ymsummorizer::ymapi {

  class client {
public:
    client(std::string user_id, std::string token);
    ~client();

    std::vector<playlist_ptr> playlist_list(std::string user_id = "");
    playlist_ptr playlist_get_info(int kind, std::string user_id = "");

    playlist_ptr playlist_create(const std::string& playlist_name, bool visibility = true);
    bool playlist_delete(int kind);
    playlist_ptr playlist_visibility(int kind, bool visibility);

    bool playlist_track_insert(playlist_ptr playlist, uint64_t track_id);
    bool playlist_track_delete(playlist_ptr playlist, uint64_t track_id, uint64_t position);

protected:
    std::string host = "https://api.music.yandex.net:443";

private:
    std::string user_id_;
    std::string token_;
  };

} // namespace ymsummorizer::ymapi
