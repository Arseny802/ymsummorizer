#include "pch.h"
#include "group_playlist.h"
#include "bot_impl.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

#include <boost/algorithm/string.hpp>
#include <fmt/format.h>

namespace ymsummorizer::tgbot::handlers {

  template class group_playlist<bot_impl>;

  template<class Base>
  group_playlist<Base>::group_playlist() { }

  template<class Base>
  group_playlist<Base>::~group_playlist() = default;

  template<class Base>
  void group_playlist<Base>::on_group_playslit_list(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    auto& bot = static_cast<Base*>(this)->get_bot();
    bot.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  template<class Base>
  void group_playlist<Base>::on_group_playslit_view(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    auto& bot = static_cast<Base*>(this)->get_bot();
    bot.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  template<class Base>
  void group_playlist<Base>::on_group_playslit_add(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_playslit_add>(message)) {
      return;
    }

    auto& bot = static_cast<Base*>(this)->get_bot();
    auto& cmd_cache = static_cast<Base*>(this)->get_callbck_cache();
    //auto run_command_callback = static_cast<Base*>(this)->run_command_callback;

    //ymsummorizer_callback_result::ptr (*run_command_callback)(const user_interaction&) = 
    //  static_cast<Base*>(this)->run_command_callback;

   /* auto& run_command_callback = std::bind(
      &bot_impl::run_command_callback<command_type::group_playslit_add>, 
      static_cast<Base*>(this), 
      std::placeholders::_1);*/

    if (cmd_cache.command_queue_clear(message->from->username)) {
      log()->warning("User {} is already in queue.", message->from->username);
      bot.getApi().sendMessage(message->chat->id, "Предыдущий запрос отклонён.");
    }

    // Shared object to pass to callback in final state
    user_interaction::ptr prolonged_interaction = std::make_shared<user_interaction>();
    prolonged_interaction->user_login_tg = message->from->username;
    prolonged_interaction->timestamp = message->date;

    bot.getApi().sendMessage(message->chat->id, "Введите название группы:");
    cmd_cache.command_queue_emplace_next(message->from->username, 
      [this, prolonged_interaction, &bot](TgBot::Message::Ptr message) {
          const auto group_name = message->text;
          if (!common::group::is_valid_name(group_name)) {
            bot.getApi().sendMessage(
                message->chat->id,
                "Название группы не подходит :(\n Попробуйте ещё раз вызвать команду с новым названием.");
            return false;
          }

          log()->info(
              "group_playslit_add - stage 1. User '{}' passed group_name '{}'.", message->from->username, group_name);
          prolonged_interaction->arguments[user_interaction::key_group_name] = std::move(group_name);

          bot.getApi().sendMessage(message->chat->id, "Введите название плейлиста:");
          return true;
        });

    cmd_cache.command_queue_emplace_next(message->from->username, 
      [this, prolonged_interaction, &bot, &cmd_cache](TgBot::Message::Ptr message) {
      std::string playlist_name = message->text;
      if (!common::playlist::is_valid_name(playlist_name)) {
        bot.getApi().sendMessage(
            message->chat->id,
            "Название плейлиста не подходит :(\n Попробуйте ещё раз вызвать команду с новым названием.");
        return false;
      }

      log()->info(
          "group_playslit_add - stage 2. User '{}' passed playlist_name '{}'.", message->from->username, playlist_name);
      prolonged_interaction->arguments[user_interaction::key_playlist_name] = std::move(playlist_name);

      ymsummorizer_callback_result::ptr result = cmd_cache.callback_commands[command_type::group_playslit_add](*prolonged_interaction);
      if (result->ok) {
        bot.getApi().sendMessage(
            message->chat->id,
            fmt::format("Плейлист '{}' добавлен в группу '{}'.",
                        prolonged_interaction->arguments[user_interaction::key_playlist_name],
                        prolonged_interaction->arguments[user_interaction::key_group_name]));
      } else {
        bot.getApi().sendMessage(
            message->chat->id,
            fmt::format("Не удалось добавить плейлист '{}' в группу '{}'.",
                        prolonged_interaction->arguments[user_interaction::key_playlist_name],
                        prolonged_interaction->arguments[user_interaction::key_group_name]));
      }
      return true;
    });
  }

  template<class Base>
  void group_playlist<Base>::on_group_playslit_remove(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    auto& bot = static_cast<Base*>(this)->get_bot();
    bot.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

} // namespace ymsummorizer::tgbot::handlers
