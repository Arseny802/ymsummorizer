#include "common/playlist_track.h"

#include "pch.h"

namespace ymsummorizer::common {

std::string playlist_track::format() const {
  return id + " " + name + " " + group_id;
}

bool playlist_track::is_valid_name(const std::string_view name) noexcept {
  if (name.empty()) {
    return false;
  }

  if (name.size() > 64) {
    return false;
  }

  return true;
}
}  // namespace ymsummorizer::common
