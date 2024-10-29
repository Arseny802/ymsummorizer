#pragma once
#include <optional>
#include <string>

namespace ymsummorizer::tgbot {
  struct user_interaction {
    std::string user_login_tg;

    std::optional<std::string> message = std::nullopt;
    std::optional<bool> poll_answer_success = std::nullopt;

    uint64_t timestamp = 0;
  };

} // namespace ymsummorizer::tgbot