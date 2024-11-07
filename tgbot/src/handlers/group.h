#pragma once
#include "../helpers/user_autorization.h"
#include "storage/db_manager.h"

#include <tgbot/tgbot.h>

namespace ymsummorizer::tgbot::handlers {

  template<class Base>
  class group: private helpers::user_autorization {
public:
    group();
    virtual ~group();

    group(const group&) = delete;
    group& operator=(const group&) = delete;

    group(group&&) = delete;
    group& operator=(group&&) = delete;

    void on_group_list(TgBot::Message::Ptr message);
    void on_group_create(TgBot::Message::Ptr message);
    void on_group_delete(TgBot::Message::Ptr message);
  };

} // namespace ymsummorizer::tgbot::handlers
