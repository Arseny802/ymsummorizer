#include "pch.h"
#include "bot_impl.h"
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>

namespace ymsummorizer::tgbot {

  bot_impl::bot_impl(std::string token, storage::db_manager& db): token_(token), bot_(token_), db_(db) {
    AUTOLOG_TG
    init_commands();
  }

  bot_impl::~bot_impl() {
    AUTOLOG_TG
  }

  void bot_impl::init_commands() {
    AUTOLOG_TG

    // User commands
    bot_.getEvents().onCommand("start", std::bind(&bot_impl::on_start, this, std::placeholders::_1));
    bot_.getEvents().onCommand("user_token_add", std::bind(&bot_impl::on_user_token_add, this, std::placeholders::_1));
    bot_.getEvents().onCommand("user_token_erase",
                               std::bind(&bot_impl::on_user_token_erase, this, std::placeholders::_1));
    bot_.getEvents().onCommand("user_view", std::bind(&bot_impl::on_user_view, this, std::placeholders::_1));
    bot_.getEvents().onCommand("group_list", std::bind(&bot_impl::on_group_list, this, std::placeholders::_1));
    bot_.getEvents().onCommand("group_leave", std::bind(&bot_impl::on_group_leave, this, std::placeholders::_1));
    bot_.getEvents().onCommand("group_playslit_list",
                               std::bind(&bot_impl::on_group_playslit_list, this, std::placeholders::_1));
    bot_.getEvents().onCommand("group_playslit_view",
                               std::bind(&bot_impl::on_group_playslit_view, this, std::placeholders::_1));

    // Group administrator commands
    bot_.getEvents().onCommand("group_user_add", std::bind(&bot_impl::on_group_user_add, this, std::placeholders::_1));
    bot_.getEvents().onCommand("group_user_remove",
                               std::bind(&bot_impl::on_group_user_remove, this, std::placeholders::_1));
    bot_.getEvents().onCommand("group_playslit_add",
                               std::bind(&bot_impl::on_group_playslit_add, this, std::placeholders::_1));
    bot_.getEvents().onCommand("group_playslit_remove",
                               std::bind(&bot_impl::on_group_playslit_remove, this, std::placeholders::_1));

    // Administrator commands
    bot_.getEvents().onCommand("group_create", std::bind(&bot_impl::on_group_create, this, std::placeholders::_1));
    bot_.getEvents().onCommand("group_delete", std::bind(&bot_impl::on_group_delete, this, std::placeholders::_1));

    // Ehco user messages
    bot_.getEvents().onNonCommandMessage(std::bind(&bot_impl::on_non_command_message, this, std::placeholders::_1));
    bot_.getEvents().onUnknownCommand([&bot = this->bot_](TgBot::Message::Ptr message) {
      log()->info("User wrote Unknown Command: '{}'.", message->text.c_str());
      bot.getApi().sendMessage(message->chat->id, "Я не знаю команды " + message->text);
    });
  }

  void bot_impl::on_start(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::start>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_start
    bot_.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void bot_impl::on_user_view(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::user_view>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_user_view
    bot_.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void bot_impl::on_user_token_add(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::user_token_add>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_user_token_add
    bot_.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void bot_impl::on_user_token_erase(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::user_token_erase>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_user_token_erase
    bot_.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void bot_impl::on_group_list(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_list>(message)) {
      return;
    }

    user_interaction ui;
    ui.user_login_tg = message->from->username;
    ui.timestamp = message->date;

    ymsummorizer_callback_result::ptr result = run_command_callback<command_type::group_list>(ui);
    if (result->ok) {
      group_list_callback_result::ptr group_list = dynamic_pointer_cast<group_list_callback_result>(result);
      if (group_list == nullptr) {
        log()->critical("group_list_callback_result is nullptr");
        return;
      }

      if (group_list->groups.empty()) {
        bot_.getApi().sendMessage(message->chat->id, "Список групп пуст.");
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
      bot_.getApi().sendMessage(message->chat->id, ss.str(), nullptr, nullptr, nullptr, "Markdown");

    } else {
      bot_.getApi().sendMessage(message->chat->id, "Не удалось получить список групп.");
    }
  }

  void bot_impl::on_group_leave(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_leave>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_leave
    bot_.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void bot_impl::on_group_playslit_list(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_playslit_list>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_playslit_list
    bot_.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void bot_impl::on_group_playslit_view(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_playslit_view>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_playslit_view
    bot_.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void bot_impl::on_group_user_add(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_add>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_add
    bot_.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void bot_impl::on_group_user_remove(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_user_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_user_remove
    bot_.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void bot_impl::on_group_playslit_add(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_playslit_add>(message)) {
      return;
    }

    if (user_command_queue_.contains(message->from->username)) {
      log()->warning("User {} is already in queue.", message->from->username);
      bot_.getApi().sendMessage(message->chat->id, "Предыдущий запрос отклонён.");
      user_command_queue_.erase(message->from->username);
    }

    // Shared object to pass to callback in final state
    user_interaction::ptr prolonged_interaction = std::make_shared<user_interaction>();
    prolonged_interaction->user_login_tg = message->from->username;
    prolonged_interaction->timestamp = message->date;

    bot_.getApi().sendMessage(message->chat->id, "Введите название группы:");
    user_command_queue_.emplace(message->from->username, callback_queue());
    user_command_queue_[message->from->username].emplace([this, prolonged_interaction](TgBot::Message::Ptr message) {
      const auto group_name = message->text;
      if (!common::group::is_valid_name(group_name)) {
        bot_.getApi().sendMessage(
            message->chat->id,
            "Название группы не подходит :(\n Попробуйте ещё раз вызвать команду с новым названием.");
        return false;
      }

      log()->info(
          "group_playslit_add - stage 1. User '{}' passed group_name '{}'.", message->from->username, group_name);
      prolonged_interaction->arguments[user_interaction::key_group_name] = std::move(group_name);

      bot_.getApi().sendMessage(message->chat->id, "Введите название плейлиста:");
      return true;
    });

    user_command_queue_[message->from->username].emplace([this, prolonged_interaction](TgBot::Message::Ptr message) {
      std::string playlist_name = message->text;
      if (!common::playlist::is_valid_name(playlist_name)) {
        bot_.getApi().sendMessage(
            message->chat->id,
            "Название плейлиста не подходит :(\n Попробуйте ещё раз вызвать команду с новым названием.");
        return false;
      }

      log()->info(
          "group_playslit_add - stage 2. User '{}' passed playlist_name '{}'.", message->from->username, playlist_name);
      prolonged_interaction->arguments[user_interaction::key_playlist_name] = std::move(playlist_name);

      ymsummorizer_callback_result::ptr result =
          run_command_callback<command_type::group_playslit_add>(*prolonged_interaction);
      if (result->ok) {
        bot_.getApi().sendMessage(message->chat->id,
                                  fmt::format("Плейлист '{}' добавлен в группу '{}'.",
                                              prolonged_interaction->arguments[user_interaction::key_playlist_name],
                                              prolonged_interaction->arguments[user_interaction::key_group_name]));
      } else {
        bot_.getApi().sendMessage(message->chat->id,
                                  fmt::format("Не удалось добавить плейлист '{}' в группу '{}'.",
                                              prolonged_interaction->arguments[user_interaction::key_playlist_name],
                                              prolonged_interaction->arguments[user_interaction::key_group_name]));
      }
      return true;
    });
  }

  void bot_impl::on_group_playslit_remove(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_playslit_remove>(message)) {
      return;
    }

    // TODO(Arseny802): implement bot_impl::on_group_playslit_remove
    bot_.getApi().sendMessage(message->chat->id, "Я пока такое не поддерживаю...");
  }

  void bot_impl::on_group_create(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_create>(message)) {
      return;
    }

    if (user_command_queue_.contains(message->from->username)) {
      log()->warning("User {} is already in queue.", message->from->username);
      bot_.getApi().sendMessage(message->chat->id, "Предыдущий запрос отклонён.");
      user_command_queue_.erase(message->from->username);
    }

    bot_.getApi().sendMessage(message->chat->id, "Введите название группы:");
    user_command_queue_.emplace(message->from->username, callback_queue());
    user_command_queue_[message->from->username].emplace([this](TgBot::Message::Ptr message) {
      log()->info("User wrote group_create: '{}'.", message->text.c_str());

      user_interaction ui;
      ui.user_login_tg = message->from->username;
      ui.timestamp = message->date;
      ui.arguments[user_interaction::key_group_name] = message->text;
      ymsummorizer_callback_result::ptr result = run_command_callback<command_type::group_create>(ui);
      if (result->ok) {
        bot_.getApi().sendMessage(message->chat->id, fmt::format("Группа '{}' создана.", message->text));
      } else {
        bot_.getApi().sendMessage(message->chat->id, fmt::format("Не удалось создать группу '{}'.", message->text));
      }

      return true;
    });
  }

  void bot_impl::on_group_delete(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG
    if (!validate_user_command<command_type::group_delete>(message)) {
      return;
    }

    if (user_command_queue_.contains(message->from->username)) {
      log()->warning("User {} is already in queue.", message->from->username);
      bot_.getApi().sendMessage(message->chat->id, "Предыдущий запрос отклонён.");
      user_command_queue_.erase(message->from->username);
    }

    bot_.getApi().sendMessage(message->chat->id, "Введите название группы:");
    user_command_queue_.emplace(message->from->username, callback_queue());
    user_command_queue_[message->from->username].emplace([this](TgBot::Message::Ptr message) {
      log()->info("User wrote group_delete: '{}'.", message->text.c_str());

      user_interaction ui;
      ui.user_login_tg = message->from->username;
      ui.timestamp = message->date;
      ui.arguments[user_interaction::key_group_name] = message->text;
      ymsummorizer_callback_result::ptr result = run_command_callback<command_type::group_delete>(ui);
      if (result->ok) {
        bot_.getApi().sendMessage(message->chat->id, fmt::format("Группа '{}' удалена.", message->text));
      } else {
        bot_.getApi().sendMessage(message->chat->id, fmt::format("Не удалось удалить группу '{}'.", message->text));
      }

      return true;
    });
  }

  void bot_impl::on_non_command_message(TgBot::Message::Ptr message) {
    AUTOMEASURE_TG

    auto command_iter = user_command_queue_.find(message->from->username);
    if (command_iter != user_command_queue_.end()) {
      log()->info("User {} wrote '{}'. Passing to user_command_queue_", message->from->username, message->text);
      auto& cmd = command_iter->second.front();
      if (!cmd(message)) {
        user_command_queue_.erase(command_iter);
        log()->warning("Command failed. User {} command queue is empty.", message->from->username);
        return;
      }

      command_iter->second.pop();
      if (command_iter->second.empty()) {
        user_command_queue_.erase(command_iter);
        log()->info("User {} command queue is empty.", message->from->username);
      }
    } else {
      // No commands found in queue -> we can't handle this message
      log()->info("User {} wrote '{}'.", message->from->username, message->text);
      bot_.getApi().sendMessage(message->chat->id, "Я не умею в обычные диалоги. Попробуйте воспользоваться командой.");
    }
  }

  template<command_type CT>
  ymsummorizer_callback_result::ptr bot_impl::run_command_callback(const user_interaction& ui) {
    AUTOLOG_TG
    if (callback_commands_.contains(CT)) {
      return callback_commands_[CT](ui);
    } else {
      log()->critical("Callback for command {} not found", static_cast<std::underlying_type_t<command_type>>(CT));
    }

    return nullptr;
  }

  bool bot_impl::start() {
    AUTOLOG_TG
    try {
      //
      //  TgLongPoll(const Api* api, const EventHandler* eventHandler, std::int32_t limit, std::int32_t timeout,
      //  std::shared_ptr<std::vector<std::string>> allowUpdates);
      //    TgLongPoll(const Bot& bot, std::int32_t limit = 100, std::int32_t timeout = 10, const
      //    std::shared_ptr<std::vector<std::string>>& allowUpdates = nullptr);

      log()->info("Bot username: {}", bot_.getApi().getMe()->username.c_str());
      TgBot::TgLongPoll longPoll(bot_);
      while (true) {
        log()->info("Long poll started");
        longPoll.start();
      }
    } catch (TgBot::TgException& e) {
      log()->error("error: {}", e.what());
    }

    return true;
  }

  bool bot_impl::stop() {
    AUTOLOG_TG
    return true;
  }

  bool bot_impl::send_message(const std::string& chat_id, const std::string& text) {
    AUTOLOG_TG
    log()->info("Sending message to {}: {}", chat_id, text);
    bot_.getApi().sendMessage(chat_id, text);
    return true;
  }

  void bot_impl::set_callback_command(
      command_type ct,
      std::function<ymsummorizer_callback_result::ptr(const user_interaction&)>&& callback) {
    AUTOLOG_TG

    if (callback_commands_.contains(ct)) {
      callback_commands_[ct] = std::move(callback);
      log()->warning("Command {} is already registered", static_cast<std::underlying_type_t<command_type>>(ct));
    } else {
      callback_commands_.emplace(ct, std::move(callback));
    }
  }

  storage::db_manager& bot_impl::get_db() const noexcept {
    return db_;
  }
} // namespace ymsummorizer::tgbot
