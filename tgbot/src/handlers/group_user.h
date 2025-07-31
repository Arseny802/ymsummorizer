#pragma once
#include <tgbot/tgbot.h>

#include "../helpers/user_autorization.h"
#include "storage/db_manager.h"

namespace ymsummorizer::tgbot::handlers {

class group_user : private helpers::user_autorization {
 public:
  group_user();
  ~group_user();

  group_user(const group_user&) = delete;
  group_user& operator=(const group_user&) = delete;

  group_user(group_user&&) = delete;
  group_user& operator=(group_user&&) = delete;

  void init_commands(const helpers::handler_context& context);

  void on_group_user_add(const helpers::handler_context& context, TgBot::Message::Ptr message);
  void on_group_user_remove(const helpers::handler_context& context, TgBot::Message::Ptr message);
  void on_group_leave(const helpers::handler_context& context, TgBot::Message::Ptr message);
};

}  // namespace ymsummorizer::tgbot::handlers
