#include "pch.h"
#include "common/playlist.h"

namespace ymsummorizer::common {

  std::string playlist::format() const {
    return id + " " + name + " " + group_id;
  }

  bool playlist::is_valid_name(const std::string_view name) noexcept {
    if (name.empty()) {
      return false;
    }

    if (name.size() > 64) {
      return false;
    }

    return true;
  }
} // namespace ymsummorizer::common
