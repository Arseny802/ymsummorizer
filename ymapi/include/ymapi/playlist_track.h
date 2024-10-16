#include "track.h"
#include <memory>
#include <string>

namespace ymsummorizer::ymapi {

  struct playlist_track: track {
    uint64_t id;
    uint64_t originalIndex;
    uint64_t timestamp;
  };

  using playlist_track_ptr = std::shared_ptr<playlist_track>;
} // namespace ymsummorizer::ymapi
