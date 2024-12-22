#include "common/user.h"

#include <unordered_set>

#include "pch.h"

namespace ymsummorizer::common {

std::string user::format() const {
  return id + " " + name + " " + login_tg + " " + login_yandex;
}

std::string user::hide_spetial_chars(const std::string& str) noexcept {
  std::string result;
  result.reserve(str.size() * 2);
  const std::unordered_set special_chars = {'_'};
  for (size_t i = 0; i < str.size(); ++i) {
    if (special_chars.find(str[i]) != special_chars.cend()) {
      result += '\\';
    }
    result += str[i];
  }

  return result;
}
}  // namespace ymsummorizer::common
