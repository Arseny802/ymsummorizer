#include "bot_info.h"
#include "hare/defs.h"
#include <nlohmann/json.hpp>

namespace ymsummorizer::storage::data {

bot_info_generator::bot_info_generator(db::manager_base_ptr db): db_(std::move(db)) {
  AUTOTRACE;
}

bot_info_generator::~bot_info_generator() {
  AUTOTRACE;
}

bool bot_info_generator::generate_bot_info() {
  try {
    common::bot_info bot_info;
    bot_info.token = "INSERT_YOUR_BOT_TOKEN_HERE";
    if (!db_->update_bot_info(bot_info)) {
      log()->error("Failed to set bot info in database.");
      return false;
    }
    return true;
  } catch (const std::exception& e) {
    log()->error("Error during bot info generation: {0}", e.what());
    return false;
  }
}

}  // namespace ymsummorizer::storage::data
