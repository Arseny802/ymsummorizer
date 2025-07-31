#include "bot_impl.h"
#include "pch.h"

#include <tgbot/tgbot.h>

namespace ymsummorizer::tgbot {

bot_impl::bot_impl(std::string token, storage::db_manager& db)
    : token_(std::move(token)), bot_(token_), db_(db) {
  AUTOTRACE
  init_commands();
}

bot_impl::~bot_impl() {
  AUTOTRACE
}

void bot_impl::init_commands() {
  AUTOTRACE

  helpers::handler_context context{bot_, db_, usrcmd_callbck_cache_};
  handlers::user::init_commands(context);
  handlers::group::init_commands(context);
  handlers::group_user::init_commands(context);
  handlers::group_playlist::init_commands(context);

  bot_.getEvents().onNonCommandMessage(
      std::bind(&bot_impl::on_non_command_message, this, std::placeholders::_1));
  bot_.getEvents().onUnknownCommand(
      std::bind(&bot_impl::on_unknown_command, this, std::placeholders::_1));
}

void bot_impl::on_non_command_message(TgBot::Message::Ptr message) {
  AUTOMEASURE

  if (!usrcmd_callbck_cache_.try_handle_command(message)) {
    // No commands found in queue -> we can't handle this message
    log()->info("User {} wrote '{}'.", message->from->username, message->text);
    bot_.getApi().sendMessage(message->chat->id,
                              "Я не умею в обычные диалоги. Попробуйте воспользоваться командой.");
  }
}

void bot_impl::on_unknown_command(TgBot::Message::Ptr message) {
  AUTOMEASURE
  log()->info("User wrote Unknown Command: '{w}'.", message->text.c_str());
  bot_.getApi().sendMessage(message->chat->id, "Я не знаю команды " + message->text);
}

bool bot_impl::start() {
  AUTOTRACE
  constexpr std::int32_t limit = 100;
  constexpr std::int32_t timeout = 60;
  is_running_ = true;

  try {
    log()->info("Bot username: {}", bot_.getApi().getMe()->username.c_str());
    TgBot::TgLongPoll longPoll(bot_, limit, timeout);
    size_t updatesCount = 1;
    while (is_running_) {
      log()->info("Long poll started for the {} time. Timeout: {}.", updatesCount++, timeout);
      longPoll.start();
    }
    log()->info("Long poll DID NOT started for the {} time. is_running_: {}.",
                updatesCount,
                (is_running_ ? "true" : "false"));
  } catch (TgBot::TgException& e) {
    log()->error("error: {}", e.what());
  }

  return true;
}

bool bot_impl::stop() {
  AUTOTRACE
  is_running_ = false;
  return true;
}

bool bot_impl::send_message(const std::string& chat_id, const std::string& text) {
  AUTOTRACE
  log()->info("Sending message to {}: {}", chat_id, text);
  bot_.getApi().sendMessage(chat_id, text);
  return true;
}

void bot_impl::set_callback_command(command_type ct, callback_cmd&& callback) {
  AUTOTRACE
  usrcmd_callbck_cache_.callback_commands[ct] = std::move(callback);
}
}  // namespace ymsummorizer::tgbot
