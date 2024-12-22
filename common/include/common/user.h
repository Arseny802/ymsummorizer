#pragma once
#include <string>

namespace ymsummorizer::common {

struct user {
  std::string id;
  std::string name;
  std::string login_tg;
  std::string login_yandex;
  std::string token;

  std::string format() const;
  static std::string hide_spetial_chars(const std::string& str) noexcept;
};
}  // namespace ymsummorizer::common
