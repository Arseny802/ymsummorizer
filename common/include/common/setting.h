#pragma once
#include <string>

namespace ymsummorizer::common {
struct setting {
  bool operator==(const setting& other) const noexcept;
  std::string key;
  std::string value;
};
}  // namespace ymsummorizer::common
