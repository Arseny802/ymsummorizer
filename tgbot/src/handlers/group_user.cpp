#include "pch.h"
#include "group_user.h"
#include "bot_impl.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

#include <boost/algorithm/string.hpp>
#include <fmt/format.h>

namespace ymsummorizer::tgbot::handlers {

  template class group_user<bot_impl>;

  template<class Base>
  group_user<Base>::group_user() { }
  template<class Base>
  group_user<Base>::~group_user() = default;

  template<class Base>
  void group_user<Base>::on_group_user_add(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    auto& bot = static_cast<Base*>(this)->get_bot();
    bot.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  template<class Base>
  void group_user<Base>::on_group_user_remove(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    auto& bot = static_cast<Base*>(this)->get_bot();
    bot.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  template<class Base>
  void group_user<Base>::on_group_leave(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    auto& bot = static_cast<Base*>(this)->get_bot();
    bot.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }
} // namespace ymsummorizer::tgbot::handlers
