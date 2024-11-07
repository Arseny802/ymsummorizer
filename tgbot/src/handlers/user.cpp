#include "pch.h"
#include "user.h"
#include "bot_impl.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

#include <boost/algorithm/string.hpp>
#include <fmt/format.h>

namespace ymsummorizer::tgbot::handlers {

  template class user<bot_impl>;

  template<class Base>
  user<Base>::user() { }

  template<class Base>
  user<Base>::~user() = default;

  template<class Base>
  void user<Base>::on_start(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    static_cast<Base*>(this)->get_bot().getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  template<class Base>
  void user<Base>::on_user_view(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    static_cast<Base*>(this)->get_bot().getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  template<class Base>
  void user<Base>::on_user_token_add(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    static_cast<Base*>(this)->get_bot().getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  template<class Base>
  void user<Base>::on_user_token_erase(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    static_cast<Base*>(this)->get_bot().getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  template<class Base>
  void user<Base>::on_token_edit(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    static_cast<Base*>(this)->get_bot().getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }
} // namespace ymsummorizer::tgbot::handlers
