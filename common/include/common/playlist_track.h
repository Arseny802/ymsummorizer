#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace ymsummorizer::common {

struct playlist_track {
  std::string id;
  std::string name;
  std::string group_id;

  struct yandex {
    std::string user_id;
    int kind;
  };

  std::vector<yandex> yandex_users;

  std::string format() const;

  static bool is_valid_name(const std::string_view name) noexcept;
};
}  // namespace ymsummorizer::common
