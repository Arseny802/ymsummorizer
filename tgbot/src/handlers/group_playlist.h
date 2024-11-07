#pragma once
#include "../helpers/user_autorization.h"
#include "storage/db_manager.h"

#include <tgbot/tgbot.h>

namespace ymsummorizer::tgbot::handlers {

  template<class Base>
  class group_playlist: private helpers::user_autorization {
public:
    group_playlist();
    virtual ~group_playlist();

    group_playlist(const group_playlist&) = delete;
    group_playlist& operator=(const group_playlist&) = delete;

    group_playlist(group_playlist&&) = delete;
    group_playlist& operator=(group_playlist&&) = delete;

    void on_group_playslit_list(TgBot::Message::Ptr message);
    void on_group_playslit_view(TgBot::Message::Ptr message);
    void on_group_playslit_add(TgBot::Message::Ptr message);
    void on_group_playslit_remove(TgBot::Message::Ptr message);
  };

} // namespace ymsummorizer::tgbot::handlers
