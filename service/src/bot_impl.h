#pragma once
#include "storage/db_manager.h"
#include "tgbot/ymsummorizer_bot.h"

#include "handlers/group.h"
#include "handlers/group_playlist.h"
#include "handlers/group_user.h"
#include "handlers/user.h"

namespace ymsummorizer::service {

  class bot_impl: public handlers::user,
                  public handlers::group,
                  public handlers::group_playlist,
                  public handlers::group_user {
public:
    bot_impl(storage::db_manager&& db);
    ~bot_impl();

    void main();

private:
    storage::db_manager db_;
    tgbot::ymsummorizer_bot bot_;
  };

} // namespace ymsummorizer::service