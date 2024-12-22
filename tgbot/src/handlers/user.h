#pragma once
#include <tgbot/tgbot.h>

#include "../helpers/handler_context.h"
#include "../helpers/user_autorization.h"
#include "storage/db_manager.h"

namespace ymsummorizer::tgbot::handlers {

class user : private helpers::user_autorization {
 public:
  user();
  ~user();

  user(const user&) = delete;
  user& operator=(const user&) = delete;

  user(user&&) = delete;
  user& operator=(user&&) = delete;

  void init_commands(const helpers::handler_context& context);

  void on_start(const helpers::handler_context& context,
                TgBot::Message::Ptr message);
  void on_user_view(const helpers::handler_context& context,
                    TgBot::Message::Ptr message);
  void on_user_token_add(const helpers::handler_context& context,
                         TgBot::Message::Ptr message);
  void on_user_token_erase(const helpers::handler_context& context,
                           TgBot::Message::Ptr message);
  void on_token_edit(const helpers::handler_context& context,
                     TgBot::Message::Ptr message);
};

}  // namespace ymsummorizer::tgbot::handlers
