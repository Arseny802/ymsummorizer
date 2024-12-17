#include "pch.h"
#include "group_playlist.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

namespace ymsummorizer::tgbot::handlers {

  group_playlist::group_playlist() { }

  group_playlist::~group_playlist() = default;

  void group_playlist::init_commands(const helpers::handler_context& context) {
    AUTOLOG_TG

    context.bot.getEvents().onCommand(
        "group_playslit_list",
        std::bind(&group_playlist::on_group_playslit_list, this, context, std::placeholders::_1));
    context.bot.getEvents().onCommand(
        "group_playslit_view",
        std::bind(&group_playlist::on_group_playslit_view, this, context, std::placeholders::_1));
    context.bot.getEvents().onCommand(
        "group_playslit_add", std::bind(&group_playlist::on_group_playslit_add, this, context, std::placeholders::_1));
    context.bot.getEvents().onCommand(
        "group_playslit_remove",
        std::bind(&group_playlist::on_group_playslit_remove, this, context, std::placeholders::_1));
  }

  void group_playlist::on_group_playslit_list(const helpers::handler_context& context, TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(context, message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    context.bot.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void group_playlist::on_group_playslit_view(const helpers::handler_context& context, TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(context, message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    context.bot.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void group_playlist::on_group_playslit_add(const helpers::handler_context& context, TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_playslit_add>(context, message)) {
      return;
    }

    if (context.cmd_cache.command_queue_clear(message->from->username)) {
      log()->warning("User {} is already in queue.", message->from->username);
      context.bot.getApi().sendMessage(message->chat->id, "Предыдущий запрос отклонён.");
    }

    // Shared object to pass to callback in final state
    user_interaction::ptr prolonged_interaction = std::make_shared<user_interaction>();
    prolonged_interaction->user_login_tg = message->from->username;
    prolonged_interaction->timestamp = message->date;

    context.bot.getApi().sendMessage(message->chat->id, "Введите название группы:");
    context.cmd_cache.command_queue_emplace_next(
        message->from->username, [this, prolonged_interaction, &context](TgBot::Message::Ptr message) {
          const auto group_name = message->text;
          if (!common::group::is_valid_name(group_name)) {
            context.bot.getApi().sendMessage(
                message->chat->id,
                "Название группы не подходит :(\n Попробуйте ещё раз вызвать команду с новым названием.");
            return false;
          }

          log()->info(
              "group_playslit_add - stage 1. User '{}' passed group_name '{}'.", message->from->username, group_name);
          prolonged_interaction->arguments[user_interaction::key_group_name] = std::move(group_name);

          context.bot.getApi().sendMessage(message->chat->id, "Введите название плейлиста:");
          return true;
        });

    context.cmd_cache.command_queue_emplace_next(
        message->from->username, [this, prolonged_interaction, &context](TgBot::Message::Ptr message) {
          std::string playlist_name = message->text;
          if (!common::playlist::is_valid_name(playlist_name)) {
            context.bot.getApi().sendMessage(
                message->chat->id,
                "Название плейлиста не подходит :(\n Попробуйте ещё раз вызвать команду с новым названием.");
            return false;
          }

          log()->info("group_playslit_add - stage 2. User '{}' passed playlist_name '{}'.",
                      message->from->username,
                      playlist_name);
          prolonged_interaction->arguments[user_interaction::key_playlist_name] = std::move(playlist_name);

          ymsummorizer_callback_result::ptr result =
              context.cmd_cache.callback_commands[command_type::group_playslit_add](*prolonged_interaction);
          if (result->ok) {
            context.bot.getApi().sendMessage(
                message->chat->id,
                fmt::format("Плейлист '{}' добавлен в группу '{}'.",
                            prolonged_interaction->arguments[user_interaction::key_playlist_name],
                            prolonged_interaction->arguments[user_interaction::key_group_name]));
          } else {
            context.bot.getApi().sendMessage(
                message->chat->id,
                fmt::format("Не удалось добавить плейлист '{}' в группу '{}'.",
                            prolonged_interaction->arguments[user_interaction::key_playlist_name],
                            prolonged_interaction->arguments[user_interaction::key_group_name]));
          }
          return true;
        });
  }

  void group_playlist::on_group_playslit_remove(const helpers::handler_context& context, TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(context, message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    context.bot.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

} // namespace ymsummorizer::tgbot::handlers
