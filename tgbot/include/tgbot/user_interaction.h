#pragma once
#include <map>
#include <memory>
#include <optional>
#include <string>

namespace ymsummorizer::tgbot {

struct user_interaction {
  using ptr = std::shared_ptr<user_interaction>;

  // Keys
  static constexpr std::string_view key_group_name = "group_name";
  static constexpr std::string_view key_playlist_name = "playlist_name";
  static constexpr std::string_view key_plogin_tg = "login_tg";
  static constexpr std::string_view key_token_yandex = "token_yandex";
  static constexpr std::string_view key_poll_answer_success = "poll_track";

  // Values
  std::map<std::string_view, std::string> arguments = {};
  std::optional<bool> poll_answer_success = std::nullopt;

  // Meta data
  std::string user_login_tg{};
  uint64_t timestamp = 0;
};

}  // namespace ymsummorizer::tgbot
