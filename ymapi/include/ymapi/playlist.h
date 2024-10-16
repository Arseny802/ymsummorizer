#include "playlist_track.h"
#include <memory>
#include <string>

namespace ymsummorizer::ymapi {

  struct playlist {
    std::string title;
    std::string description;
    std::string playlistUuid;

    uint64_t uid;
    uint64_t kind;
    bool visibility;
    uint64_t revision;
    uint64_t snapshot;

    bool available;
    uint64_t trackCount;
    uint64_t durationMs;

    uint64_t created;
    uint64_t modified;
    uint64_t likesCount;

    struct owner_t {
      uint64_t uid;
      std::string login;
      std::string name;
    } owner;

    struct pager_t {
      uint64_t total;
      uint64_t page;
      uint64_t perPage;
    } pager;

    std::vector<playlist_track> tracks;
  };

  using playlist_ptr = std::shared_ptr<playlist>;

  void log_playlist(playlist_ptr playlist);
  void log_playlist(playlist playlist);
} // namespace ymsummorizer::ymapi
