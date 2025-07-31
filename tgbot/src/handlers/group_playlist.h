#pragma once
#include <tgbot/tgbot.h>

#include "../helpers/user_autorization.h"
#include "storage/db_manager.h"

namespace ymsummorizer::tgbot::handlers {

class group_playlist : private helpers::user_autorization {
 public:
  group_playlist();
  ~group_playlist();

  group_playlist(const group_playlist&) = delete;
  group_playlist& operator=(const group_playlist&) = delete;

  group_playlist(group_playlist&&) = delete;
  group_playlist& operator=(group_playlist&&) = delete;

  void init_commands(const helpers::handler_context& context);

  void on_group_playslit_list(const helpers::handler_context& context, TgBot::Message::Ptr message);
  void on_group_playslit_view(const helpers::handler_context& context, TgBot::Message::Ptr message);
  void on_group_playslit_add(const helpers::handler_context& context, TgBot::Message::Ptr message);
  void on_group_playslit_remove(const helpers::handler_context& context,
                                TgBot::Message::Ptr message);
};

}  // namespace ymsummorizer::tgbot::handlers
