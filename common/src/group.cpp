#include "pch.h"
#include "common/group.h"

namespace ymsummorizer::common {

  std::string group::format() const {
    return name + " ('" + id + "') with " + std::to_string(user_ids.size()) + " users.";
  }

  bool group::is_valid_name(const std::string_view name) noexcept {
    if (name.empty()) {
      return false;
    }

    if (name.size() > 64) {
      return false;
    }

    return true;
  }
} // namespace ymsummorizer::common
