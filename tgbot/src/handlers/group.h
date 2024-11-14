#pragma once
#include "../helpers/user_autorization.h"
#include "storage/db_manager.h"

#include <tgbot/tgbot.h>

namespace ymsummorizer::tgbot::handlers {

  class group: private helpers::user_autorization {
public:
    group();
    ~group();

    group(const group&) = delete;
    group& operator=(const group&) = delete;

    group(group&&) = delete;
    group& operator=(group&&) = delete;

    void init_commands(const helpers::handler_context& context);

    void on_group_list(const helpers::handler_context& context, TgBot::Message::Ptr message);
    void on_group_create(const helpers::handler_context& context, TgBot::Message::Ptr message);
    void on_group_delete(const helpers::handler_context& context, TgBot::Message::Ptr message);
  };

} // namespace ymsummorizer::tgbot::handlers
