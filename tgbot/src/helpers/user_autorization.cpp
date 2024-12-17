#include "pch.h"
#include "user_autorization.h"

namespace ymsummorizer::tgbot::helpers {

  user_autorization::user_autorization() = default;
  user_autorization::~user_autorization() = default;

  bool user_autorization::validate_user(const helpers::handler_context& context, TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (message->from->isBot) {
      log()->info("User {} is a bot\n", message->from->username.c_str());
      return false;
    }

    const auto& current_user = message->from->username;

    // Use it for debug purposes
    constexpr std::array user_whitelist = {"arseny802"};
    if constexpr (!user_whitelist.empty()) {
      if (std::none_of(user_whitelist.cbegin(), user_whitelist.cend(), [&current_user](const auto& user) {
            return user == boost::algorithm::to_lower_copy(current_user);
          })) {
        log()->info("User {} is not in whitelist\n", message->from->username.c_str());
        return false;
      }
    }

    // TODO: load user info from storage
    // TODO: validate current command by mask

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::start>(const helpers::handler_context& context,
                                                                     TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::user_token_add>(const helpers::handler_context& context,
                                                                              TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::user_token_erase>(const helpers::handler_context& context,
                                                                                TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::user_view>(const helpers::handler_context& context,
                                                                         TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::group_list>(const helpers::handler_context& context,
                                                                          TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::group_create>(const helpers::handler_context& context,
                                                                            TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::group_delete>(const helpers::handler_context& context,
                                                                            TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::group_user_add>(const helpers::handler_context& context,
                                                                              TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::group_user_remove>(
      const helpers::handler_context& context,
      TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::group_leave>(const helpers::handler_context& context,
                                                                           TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::group_playslit_list>(
      const helpers::handler_context& context,
      TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::group_playslit_view>(
      const helpers::handler_context& context,
      TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::group_playslit_add>(
      const helpers::handler_context& context,
      TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<>
  bool user_autorization::validate_user_command<command_type::group_playslit_remove>(
      const helpers::handler_context& context,
      TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user(context, message)) {
      return false;
    }

    return true;
  }

  template<command_type CT>
  bool user_autorization::validate_user_command(const helpers::handler_context& context, TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    return validate_user(context, message);
  }

} // namespace ymsummorizer::tgbot::helpers
