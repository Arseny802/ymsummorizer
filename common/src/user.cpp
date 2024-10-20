#include "pch.h"
#include "common/user.h"

namespace ymsummorizer::common {

  std::string user::format() const {
    return id + " " + name + " " + login_tg + " " + login_yandex;
  }
} // namespace ymsummorizer::common
