#pragma once
#include "storage/db_manager.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

namespace ymsummorizer::service::handlers {

  class group_playlist {
public:
    group_playlist(storage::db_manager& db);
    virtual ~group_playlist();

    group_playlist(const group_playlist&) = delete;
    group_playlist& operator=(const group_playlist&) = delete;

    group_playlist(group_playlist&&) = delete;
    group_playlist& operator=(group_playlist&&) = delete;

    tgbot::ymsummorizer_callback_result::ptr on_group_playslit_list(const tgbot::user_interaction& ui);
    tgbot::ymsummorizer_callback_result::ptr on_group_playslit_view(const tgbot::user_interaction& ui);
    tgbot::ymsummorizer_callback_result::ptr on_group_playslit_add(const tgbot::user_interaction& ui);
    tgbot::ymsummorizer_callback_result::ptr on_group_playslit_remove(const tgbot::user_interaction& ui);

private:
    storage::db_manager& db_;
  };

} // namespace ymsummorizer::service::handlers
