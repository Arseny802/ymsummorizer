#include "pch.h"
#include "bot_impl.h"

#include "storage/db_manager.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_bot.h"
#include "ymapi/client.h"

int main() {
  ymsummorizer::service::initialize_logging();
  ymsummorizer::storage::db_manager db(ymsummorizer::storage::storage_types::cfg_json, "ymsummorizer.storage.json");
  if (!db.connect())
    return EXIT_FAILURE; // TODO: return error code instead of exit code.

  ymsummorizer::service::bot_impl bot_impl(std::move(db));

  bot_impl.main();

  return EXIT_SUCCESS;
}
