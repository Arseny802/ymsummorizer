#include "group_user.h"

#include "bot_impl.h"
#include "pch.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

namespace ymsummorizer::tgbot::handlers {

group_user::group_user() {}
group_user::~group_user() = default;

void group_user::init_commands(const helpers::handler_context& context) {
  AUTOLOG_TG

  context.bot.getEvents().onCommand(
      "group_user_add", std::bind(&group_user::on_group_user_add, this, context,
                                  std::placeholders::_1));
  context.bot.getEvents().onCommand(
      "group_user_remove", std::bind(&group_user::on_group_user_remove, this,
                                     context, std::placeholders::_1));
  context.bot.getEvents().onCommand(
      "group_leave", std::bind(&group_user::on_group_leave, this, context,
                               std::placeholders::_1));
}

void group_user::on_group_user_add(const helpers::handler_context& context,
                                   TgBot::Message::Ptr message) {
  AUTOMEASURE_TG
  if (!validate_user_command<command_type::group_user_add>(context, message)) {
    return;
  }

  // TODO(Arseny802): implement bot_impl::on_group_user_add
  context.bot.getApi().sendMessage(message->chat->id,
                                   "Я пока такое не поддерживаю...");
}

void group_user::on_group_user_remove(const helpers::handler_context& context,
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

void group_user::on_group_leave(const helpers::handler_context& context,
                                TgBot::Message::Ptr message) {
  AUTOMEASURE_TG
  if (!validate_user_command<command_type::group_leave>(context, message)) {
    return;
  }

  // TODO(Arseny802): implement bot_impl::on_group_leave
  context.bot.getApi().sendMessage(message->chat->id,
                                   "Я пока такое не поддерживаю...");
}
}  // namespace ymsummorizer::tgbot::handlers
