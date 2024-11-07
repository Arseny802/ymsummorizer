#pragma once
#include "../helpers/user_autorization.h"
#include "storage/db_manager.h"

#include <tgbot/tgbot.h>

namespace ymsummorizer::tgbot::handlers {

  template<class Base>
  class group_user: private helpers::user_autorization {
public:
    group_user();
    virtual ~group_user();

    group_user(const group_user&) = delete;
    group_user& operator=(const group_user&) = delete;

    group_user(group_user&&) = delete;
    group_user& operator=(group_user&&) = delete;

    void on_group_user_add(TgBot::Message::Ptr message);
    void on_group_user_remove(TgBot::Message::Ptr message);
    void on_group_leave(TgBot::Message::Ptr message);
  };

} // namespace ymsummorizer::tgbot::handlers
