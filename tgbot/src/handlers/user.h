#pragma once
#include "../helpers/user_autorization.h"
#include "storage/db_manager.h"

#include <tgbot/tgbot.h>

namespace ymsummorizer::tgbot::handlers {

  template<class Base>
  class user: private helpers::user_autorization {
public:
    user();
    virtual ~user();

    user(const user&) = delete;
    user& operator=(const user&) = delete;

    user(user&&) = delete;
    user& operator=(user&&) = delete;

    void on_start(TgBot::Message::Ptr message);
    void on_user_view(TgBot::Message::Ptr message);
    void on_user_token_add(TgBot::Message::Ptr message);
    void on_user_token_erase(TgBot::Message::Ptr message);
    void on_token_edit(TgBot::Message::Ptr message);
  };

} // namespace ymsummorizer::tgbot::handlers
