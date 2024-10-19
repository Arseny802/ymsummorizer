#pragma once
#include "album.h"
#include "artist.h"
#include <memory>
#include <string>
#include <vector>

namespace ymsummorizer::ymapi {

  struct track {
    uint64_t id;
    uint64_t realId;
    std::string title;
    std::string version;

    bool available;
    uint64_t durationMs;

    std::vector<artist> artists;
    std::vector<album> albums;
  };

  using track_ptr = std::shared_ptr<track>;
} // namespace ymsummorizer::ymapi
