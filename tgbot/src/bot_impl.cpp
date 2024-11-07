#include "pch.h"
#include "bot_impl.h"
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>

namespace ymsummorizer::tgbot {

  bot_impl::bot_impl(std::string token, storage::db_manager& db):
      handlers::user<bot_impl>(),
      handlers::group<bot_impl>(),
      handlers::group_playlist<bot_impl>(),
      handlers::group_user<bot_impl>(),
      token_(token),
      bot_(token_),
      db_(db) {
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

  void bot_impl::on_start(TgBot::Message::Ptr message) { }

  void bot_impl::on_user_view(TgBot::Message::Ptr message) { }

  void bot_impl::on_user_token_add(TgBot::Message::Ptr message) { }

  void bot_impl::on_user_token_erase(TgBot::Message::Ptr message) { }

  void bot_impl::on_group_list(TgBot::Message::Ptr message) { }

  void bot_impl::on_group_leave(TgBot::Message::Ptr message) { }

  void bot_impl::on_group_playslit_list(TgBot::Message::Ptr message) { }

  void bot_impl::on_group_playslit_view(TgBot::Message::Ptr message) { }

  void bot_impl::on_group_user_add(TgBot::Message::Ptr message) { }

  void bot_impl::on_group_user_remove(TgBot::Message::Ptr message) { }

  void bot_impl::on_group_playslit_add(TgBot::Message::Ptr message) { }

  void bot_impl::on_group_playslit_remove(TgBot::Message::Ptr message) { }

  void bot_impl::on_group_create(TgBot::Message::Ptr message) { }

  void bot_impl::on_group_delete(TgBot::Message::Ptr message) { }

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
    constexpr std::int32_t limit = 100;
    constexpr std::int32_t timeout = 60;

    try {
      log()->info("Bot username: {}", bot_.getApi().getMe()->username.c_str());
      TgBot::TgLongPoll longPoll(bot_, limit, timeout);
      size_t updatesCount = 1;
      while (true) {
        log()->info("Long poll started for the {} time. Timeout: {}.", updatesCount++, timeout);
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
} // namespace ymsummorizer::tgbot
