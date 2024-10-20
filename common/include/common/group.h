#pragma once
#include <string>
#include <vector>

namespace ymsummorizer::common {

  struct group {
    std::string id;
    std::string name;
    std::vector<std::string> user_ids;

    std::string format() const;
  };
} // namespace ymsummorizer::common
