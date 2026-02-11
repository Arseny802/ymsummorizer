#include "pch.h"
#include <boost/process/environment.hpp>

#include "bot_impl.h"

#include "storage/storage.hpp"
#include "tgbot/tgbot.hpp"
#include "ymapi/ymapi.hpp"

void initialize_logging() {
  log()->info("Application started. PID: {}", boost::this_process::get_id());
  ymsummorizer::storage::initialize_logging();
  ymsummorizer::tgbot::initialize_logging();
  ymsummorizer::ymapi::initialize_logging();
}

int main() {
  initialize_logging();
  ymsummorizer::storage::db_manager db(ymsummorizer::storage::storage_types::cfg_json, "ymsummorizer.storage.json");
  if (!db.connect())
    return EXIT_FAILURE;  // TODO: return error code instead of exit code.

  ymsummorizer::service::bot_impl bot_impl(std::move(db));

  bot_impl.main();

  return EXIT_SUCCESS;
}
