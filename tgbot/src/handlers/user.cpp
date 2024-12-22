#include "user.h"

#include "pch.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

namespace ymsummorizer::tgbot::handlers {

user::user() {}
user::~user() = default;

void user::init_commands(const helpers::handler_context& context) {
  AUTOLOG_TG

  context.bot.getEvents().onCommand(
      "start",
      std::bind(&user::on_start, this, context, std::placeholders::_1));
  context.bot.getEvents().onCommand(
      "user_token_add", std::bind(&user::on_user_token_add, this, context,
                                  std::placeholders::_1));
  context.bot.getEvents().onCommand(
      "user_token_erase", std::bind(&user::on_user_token_erase, this, context,
                                    std::placeholders::_1));
  context.bot.getEvents().onCommand(
      "user_view",
      std::bind(&user::on_user_view, this, context, std::placeholders::_1));
}

void user::on_start(const helpers::handler_context& context,
                    TgBot::Message::Ptr message) {
  AUTOMEASURE_TG
  if (!validate_user_command<command_type::group_user_remove>(context,
                                                              message)) {
    return;
  }

  // TODO(Arseny802): implement bot_impl::on_group_user_remove
  context.bot.getApi().sendMessage(message->chat->id,
                                   "Я пока такое не поддерживаю...");
}

void user::on_user_view(const helpers::handler_context& context,
                        TgBot::Message::Ptr message) {
  AUTOMEASURE_TG
  if (!validate_user_command<command_type::group_user_remove>(context,
                                                              message)) {
    return;
  }

  // TODO(Arseny802): implement bot_impl::on_group_user_remove
  context.bot.getApi().sendMessage(message->chat->id,
                                   "Я пока такое не поддерживаю...");
}

void user::on_user_token_add(const helpers::handler_context& context,
                             TgBot::Message::Ptr message) {
  AUTOMEASURE_TG
  if (!validate_user_command<command_type::group_user_remove>(context,
                                                              message)) {
    return;
  }

  // TODO(Arseny802): implement bot_impl::on_group_user_remove
  context.bot.getApi().sendMessage(message->chat->id,
                                   "Я пока такое не поддерживаю...");
}

void user::on_user_token_erase(const helpers::handler_context& context,
                               TgBot::Message::Ptr message) {
  AUTOMEASURE_TG
  if (!validate_user_command<command_type::group_user_remove>(context,
                                                              message)) {
    return;
  }

  // TODO(Arseny802): implement bot_impl::on_group_user_remove
  context.bot.getApi().sendMessage(message->chat->id,
                                   "Я пока такое не поддерживаю...");
}

void user::on_token_edit(const helpers::handler_context& context,
                         TgBot::Message::Ptr message) {
  AUTOMEASURE_TG
  if (!validate_user_command<command_type::group_user_remove>(context,
                                                              message)) {
    return;
  }

  // TODO(Arseny802): implement bot_impl::on_group_user_remove
  context.bot.getApi().sendMessage(message->chat->id,
                                   "Я пока такое не поддерживаю...");
}
}  // namespace ymsummorizer::tgbot::handlers
