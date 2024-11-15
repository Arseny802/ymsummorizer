#pragma once
#include "playlist.h"

#include <string>
#include <string_view>
#include <vector>

namespace ymsummorizer::common {

  struct group {
    std::string id;
    std::string name;
    std::vector<std::string> user_ids;
    std::vector<playlist> playlists;

    std::string format() const;

    static bool is_valid_name(const std::string_view name) noexcept;
  };
} // namespace ymsummorizer::common
