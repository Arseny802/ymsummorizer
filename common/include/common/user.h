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
  };
} // namespace ymsummorizer::common
