#include "pch.h"
#include "tgbot/ymsummorizer_bot.h"
#include "bot_impl.h"

namespace ymsummorizer::tgbot {

  ymsummorizer_bot::ymsummorizer_bot(storage::db_manager& db) {
    AUTOLOG_TG

    db.connect();

    auto bot_info = db.get_bot_info();
    if (!bot_info.has_value()) {
      log()->error("Failed to get bot info");
      return;
    }

    bot_ = std::make_unique<bot_impl>(bot_info->token, db);
  }

  ymsummorizer_bot::ymsummorizer_bot(std::string token, storage::db_manager& db) {
    AUTOLOG_TG

    bot_ = std::make_unique<bot_impl>(std::move(token), db);
  }

  ymsummorizer_bot::~ymsummorizer_bot() {
    AUTOLOG_TG
  }

  bool ymsummorizer_bot::start() {
    AUTOLOG_TG
    if (!bot_) {
      throw std::runtime_error("bot is not initialized");
    }

    return bot_->start();
  }

  bool ymsummorizer_bot::stop() {
    AUTOLOG_TG
    if (!bot_) {
      throw std::runtime_error("bot is not initialized");
    }

    return bot_->stop();
  }

  bool ymsummorizer_bot::send_message(const std::string& chat_id, const std::string& text) {
    AUTOLOG_TG
    return bot_->send_message(chat_id, text);
  }

  void ymsummorizer_bot::set_callback_command(
      command_type ct,
      std::function<ymsummorizer_callback_result::ptr(const user_interaction&)>&& callback) {
    AUTOLOG_TG
    bot_->set_callback_command(ct, std::move(callback));
  }
} // namespace ymsummorizer::tgbot
