#include "group.h"

#include "pch.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

namespace ymsummorizer::tgbot::handlers {

group::group() {}
group::~group() = default;

void group::init_commands(const helpers::handler_context& context) {
  AUTOLOG_TG

  context.bot.getEvents().onCommand(
      "group_list",
      std::bind(&group::on_group_list, this, context, std::placeholders::_1));
  context.bot.getEvents().onCommand(
      "group_create",
      std::bind(&group::on_group_create, this, context, std::placeholders::_1));
  context.bot.getEvents().onCommand(
      "group_delete",
      std::bind(&group::on_group_delete, this, context, std::placeholders::_1));
}

void group::on_group_list(const helpers::handler_context& context,
                          TgBot::Message::Ptr message) {
  AUTOMEASURE_TG
  if (!validate_user_command<command_type::group_list>(context, message)) {
    return;
  }

  user_interaction ui;
  ui.user_login_tg = message->from->username;
  ui.timestamp = message->date;

  ymsummorizer_callback_result::ptr result =
      context.cmd_cache.callback_commands[command_type::group_list](ui);
  if (result->ok) {
    group_list_callback_result::ptr group_list =
        dynamic_pointer_cast<group_list_callback_result>(result);
    if (group_list == nullptr) {
      log()->critical("group_list_callback_result is nullptr");
      return;
    }

    if (group_list->groups.empty()) {
      context.bot.getApi().sendMessage(message->chat->id, "Список групп пуст.");
      return;
    }

    std::stringstream ss;
    ss << "*Список групп*:\n\n";
    for (auto& group : group_list->groups) {
      ss << "# *" << group.name << "* _" << group.id << "_" << "\n";
      ss << "Пользователи: \n";
      for (auto& user_id : group.user_ids) {
        const auto user_it = group_list->filtered_users.find(user_id);
        if (user_it == group_list->filtered_users.end()) {
          log()->error("User {} not found in filtered_users", user_id);
          continue;
        }

        ss << " - _" << common::user::hide_spetial_chars(user_it->second.name)
           << "_ (https://t.me/"
           << common::user::hide_spetial_chars(user_it->second.login_tg) << ")"
           << "\n";
      }
      ss << "\n\n";
    }

    auto msg = ss.str();
    log()->debug(msg);
    context.bot.getApi().sendMessage(message->chat->id, msg, nullptr, nullptr,
                                     nullptr, "Markdown");

  } else {
    context.bot.getApi().sendMessage(message->chat->id,
                                     "Не удалось получить список групп.");
  }
}

void group::on_group_create(const helpers::handler_context& context,
                            TgBot::Message::Ptr message) {
  AUTOMEASURE_TG
  if (!validate_user_command<command_type::group_create>(context, message)) {
    return;
  }

  if (context.cmd_cache.command_queue_clear(message->from->username)) {
    log()->warning("User {} is already in queue.", message->from->username);
    context.bot.getApi().sendMessage(message->chat->id,
                                     "Предыдущий запрос отклонён.");
  }

  context.bot.getApi().sendMessage(message->chat->id,
                                   "Введите название группы:");
  context.cmd_cache.command_queue_emplace_next(
      message->from->username, [this, &context](TgBot::Message::Ptr message) {
        log()->info("User wrote group_create: '{}'.", message->text.c_str());

        user_interaction ui;
        ui.user_login_tg = message->from->username;
        ui.timestamp = message->date;
        ui.arguments[user_interaction::key_group_name] = message->text;
        ymsummorizer_callback_result::ptr result =
            context.cmd_cache.callback_commands[command_type::group_create](ui);
        if (result->ok) {
          context.bot.getApi().sendMessage(
              message->chat->id,
              fmt::format("Группа '{}' создана.", message->text));
        } else {
          context.bot.getApi().sendMessage(
              message->chat->id,
              fmt::format("Не удалось создать группу '{}'.", message->text));
        }

        return true;
      });
}

void group::on_group_delete(const helpers::handler_context& context,
                            TgBot::Message::Ptr message) {
  AUTOMEASURE_TG
  if (!validate_user_command<command_type::group_delete>(context, message)) {
    return;
  }

  if (context.cmd_cache.command_queue_clear(message->from->username)) {
    log()->warning("User {} is already in queue.", message->from->username);
    context.bot.getApi().sendMessage(message->chat->id,
                                     "Предыдущий запрос отклонён.");
  }

  context.bot.getApi().sendMessage(message->chat->id,
                                   "Введите название группы:");
  context.cmd_cache.command_queue_emplace_next(
      message->from->username, [this, &context](TgBot::Message::Ptr message) {
        log()->info("User wrote group_delete: '{}'.", message->text.c_str());

        user_interaction ui;
        ui.user_login_tg = message->from->username;
        ui.timestamp = message->date;
        ui.arguments[user_interaction::key_group_name] = message->text;
        ymsummorizer_callback_result::ptr result =
            context.cmd_cache.callback_commands[command_type::group_delete](ui);
        if (result->ok) {
          context.bot.getApi().sendMessage(
              message->chat->id,
              fmt::format("Группа '{}' удалена.", message->text));
        } else {
          context.bot.getApi().sendMessage(
              message->chat->id,
              fmt::format("Не удалось удалить группу '{}'.", message->text));
        }

        return true;
      });
}

}  // namespace ymsummorizer::tgbot::handlers
