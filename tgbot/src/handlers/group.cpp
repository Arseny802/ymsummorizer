#include "pch.h"
#include "group.h"
#include "bot_impl.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

#include <boost/algorithm/string.hpp>
#include <fmt/format.h>

namespace ymsummorizer::tgbot::handlers {

  template class group<bot_impl>;

  template<class Base>
  group<Base>::group() {}
  template<class Base>
  group<Base>::~group() = default;

  template<class Base>
  void group<Base>::on_group_list(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_list>(message)) {
      return;
    }
    auto& bot = static_cast<Base*>(this)->get_bot();
    auto& cmd_cache = static_cast<Base*>(this)->get_callbck_cache();
    //auto& run_command_callback = static_cast<Base*>(this)->run_command_callback;
    /*auto run_command_callback = std::bind(
      &(template static_cast<Base*>(this)->run_command_callback<command_type::group_list>), 
      this, std::placeholders::_1);*/

    user_interaction ui;
    ui.user_login_tg = message->from->username;
    ui.timestamp = message->date;

    ymsummorizer_callback_result::ptr result = cmd_cache.callback_commands[command_type::group_list](ui);
    if (result->ok) {
      group_list_callback_result::ptr group_list = dynamic_pointer_cast<group_list_callback_result>(result);
      if (group_list == nullptr) {
        log()->critical("group_list_callback_result is nullptr");
        return;
      }

      if (group_list->groups.empty()) {
        bot.getApi().sendMessage(message->chat->id, "Список групп пуст.");
        return;
      }

      std::stringstream ss;
      ss << "*Список групп*:\n\n";
      for (auto& group: group_list->groups) {
        ss << "# *" << group.name << "* _" << group.id << "_" << "\n";
        ss << "Пользователи: \n";
        for (auto& user_id: group.user_ids) {
          const auto user_it = group_list->filtered_users.find(user_id);
          if (user_it == group_list->filtered_users.end()) {
            log()->error("User {} not found in filtered_users", user_id);
            continue;
          }

          ss << " - _" << user_it->second.name << "_ (https://t.me/" << user_it->second.login_tg << ")" << "\n";
        }
        ss << "\n\n";
      }
      bot.getApi().sendMessage(message->chat->id, ss.str(), nullptr, nullptr, nullptr, "Markdown");

    } else {
      bot.getApi().sendMessage(message->chat->id, "Не удалось получить список групп.");
    }
  }

  template<class Base>
  void group<Base>::on_group_create(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_create>(message)) {
      return;
    }

    auto& bot = static_cast<Base*>(this)->get_bot();
    auto& cmd_cache = static_cast<Base*>(this)->get_callbck_cache();
    //auto& run_command_callback = static_cast<Base*>(this)->run_command_callback;
   /* auto run_command_callback = std::bind(
      &(template static_cast<Base*>(this)->run_command_callback<command_type::group_create>), 
      this, std::placeholders::_1);*/

    if (cmd_cache.command_queue_clear(message->from->username)) {
      log()->warning("User {} is already in queue.", message->from->username);
      bot.getApi().sendMessage(message->chat->id, "Предыдущий запрос отклонён.");
    }

    bot.getApi().sendMessage(message->chat->id, "Введите название группы:");
    cmd_cache.command_queue_emplace_next(message->from->username, [this, &bot, &cmd_cache](TgBot::Message::Ptr message) {
      log()->info("User wrote group_create: '{}'.", message->text.c_str());

      user_interaction ui;
      ui.user_login_tg = message->from->username;
      ui.timestamp = message->date;
      ui.arguments[user_interaction::key_group_name] = message->text;
      ymsummorizer_callback_result::ptr result = cmd_cache.callback_commands[command_type::group_create](ui);
      if (result->ok) {
        bot.getApi().sendMessage(message->chat->id, fmt::format("Группа '{}' создана.", message->text));
      } else {
        bot.getApi().sendMessage(message->chat->id, fmt::format("Не удалось создать группу '{}'.", message->text));
      }

      return true;
    });
  }

  template<class Base>
  void group<Base>::on_group_delete(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_delete>(message)) {
      return;
    }

    auto& bot = static_cast<Base*>(this)->get_bot();
    auto& cmd_cache = static_cast<Base*>(this)->get_callbck_cache();
    //auto& run_command_callback = static_cast<Base*>(this)->run_command_callback;
    /*auto run_command_callback = std::bind(
      &(template static_cast<Base*>(this)->run_command_callback<command_type::group_delete>), 
      this, std::placeholders::_1);*/

    if (cmd_cache.command_queue_clear(message->from->username)) {
      log()->warning("User {} is already in queue.", message->from->username);
      bot.getApi().sendMessage(message->chat->id, "Предыдущий запрос отклонён.");
    }

    bot.getApi().sendMessage(message->chat->id, "Введите название группы:");
    cmd_cache.command_queue_emplace_next(message->from->username, [this, &bot, &cmd_cache](TgBot::Message::Ptr message) {
      log()->info("User wrote group_delete: '{}'.", message->text.c_str());

      user_interaction ui;
      ui.user_login_tg = message->from->username;
      ui.timestamp = message->date;
      ui.arguments[user_interaction::key_group_name] = message->text;
      ymsummorizer_callback_result::ptr result = cmd_cache.callback_commands[command_type::group_delete](ui);
      if (result->ok) {
        bot.getApi().sendMessage(message->chat->id, fmt::format("Группа '{}' удалена.", message->text));
      } else {
        bot.getApi().sendMessage(message->chat->id, fmt::format("Не удалось удалить группу '{}'.", message->text));
      }

      return true;
    });
  }

} // namespace ymsummorizer::tgbot::handlers
