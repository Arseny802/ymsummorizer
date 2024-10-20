#include "pch.h"
#include "common/group.h"

namespace ymsummorizer::common {

  std::string group::format() const {
    return name + " ('" + id + "') with " + std::to_string(user_ids.size()) + " users.";
  }
} // namespace ymsummorizer::common
