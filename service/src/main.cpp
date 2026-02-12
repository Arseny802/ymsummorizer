#include "pch.h"
#include <boost/process/environment.hpp>
#include <tuple>

#include "bot_impl.h"

#include "storage/storage.hpp"
#include "tgbot/tgbot.hpp"
#include "ymapi/ymapi.hpp"

namespace ymsummorizer::service {

void initialize_logging() {
  log()->info("Application started. PID: {}", boost::this_process::get_id());
  std::ignore = storage::get_logger();
  std::ignore = tgbot::get_logger();
  std::ignore = ymapi::get_logger();
}

int main() {
  storage::db_manager db(storage::storage_types::cfg_json, "ymsummorizer.storage.json");
  if (!db.connect()) {
    return EXIT_FAILURE;  // TODO: return error code instead of exit code.
  }

  bot_impl bot_impl(std::move(db));

  bot_impl.main();
  return EXIT_SUCCESS;
}
}  // namespace ymsummorizer::service

int main() {
  ymsummorizer::service::initialize_logging();
  return ymsummorizer::service::main();
}
