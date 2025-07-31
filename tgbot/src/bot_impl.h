#pragma once
#include "storage/db_manager.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

#include "handlers/group.h"
#include "handlers/group_playlist.h"
#include "handlers/group_user.h"
#include "handlers/user.h"
#include "helpers/handler_context.h"
#include "helpers/user_autorization.h"
#include "helpers/usrcmd_callbck_cache.h"

#include <tgbot/tgbot.h>

namespace ymsummorizer::tgbot {

class bot_impl final : public handlers::user,
                       public handlers::group,
                       public handlers::group_playlist,
                       public handlers::group_user {
public:
  using callback_cmd = std::function<ymsummorizer_callback_result::ptr(
      const user_interaction &)>;

  bot_impl(std::string token, storage::db_manager &db);
  ~bot_impl();

  bot_impl() = delete;
  bot_impl(const bot_impl &) = delete;
  bot_impl(bot_impl &&) = delete;
  bot_impl operator=(const bot_impl &) = delete;
  bot_impl operator=(bot_impl &&) = delete;

  bool start();
  bool stop();

  bool send_message(const std::string &chat_id, const std::string &text);
  void set_callback_command(command_type ct, callback_cmd &&callback);

private:
  void on_non_command_message(TgBot::Message::Ptr message);
  void on_unknown_command(TgBot::Message::Ptr message);

  void init_commands();

  std::atomic_bool is_running_ = false;
  std::string token_;
  TgBot::Bot bot_;
  storage::db_manager &db_;
  helpers::usrcmd_callbck_cache usrcmd_callbck_cache_;
};

} // namespace ymsummorizer::tgbot
