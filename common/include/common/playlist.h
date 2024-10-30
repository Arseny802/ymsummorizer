#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace ymsummorizer::common {

  struct playlist {
    std::string id;
    std::string name;
    std::string group_id;

    std::string format() const;

    static bool is_valid_name(const std::string_view name) noexcept;
  };
} // namespace ymsummorizer::common
