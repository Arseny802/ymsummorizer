#pragma once
#include <tgbot/tgbot.h>

#include "storage/db_manager.h"
#include "usrcmd_callbck_cache.h"

namespace ymsummorizer::tgbot::helpers {

struct handler_context {
  TgBot::Bot& bot;
  storage::db_manager& db;
  usrcmd_callbck_cache& cmd_cache;
};
}  // namespace ymsummorizer::tgbot::helpers
