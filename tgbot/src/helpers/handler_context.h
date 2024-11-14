#pragma once
#include "usrcmd_callbck_cache.h"

#include "storage/db_manager.h"
#include <tgbot/tgbot.h>

namespace ymsummorizer::tgbot::helpers {

  struct handler_context {
    TgBot::Bot& bot;
    storage::db_manager& db;
    usrcmd_callbck_cache& cmd_cache;
  };
} // namespace ymsummorizer::tgbot::helpers
