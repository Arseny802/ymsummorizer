#include "pch.h"
// #include <tgbot/tgbot.h>

namespace yw::master_bot {
int bot_example() {
  /*TgBot::Bot bot("TOKEN");
  bot.getEvents().onCommand(
      "start", [&bot](TgBot::Message::Ptr message) {
  bot.getApi().sendMessage(message->chat->id, "Hi!"); });
  bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
    log()->info("User wrote {}\n", message->text.c_str());
    if (StringTools::startsWith(message->text, "/start")) {
      return;
    }
    bot.getApi().sendMessage(message->chat->id, "Your message is: " +
  message->text);
  });
  try {
    log()->info("Bot username: {}\n", bot.getApi().getMe()->username.c_str());
    TgBot::TgLongPoll longPoll(bot);
    while (true) {
      log()->info("Long poll started\n");
      longPoll.start();
    }
  } catch (TgBot::TgException& e) {
    log()->error("error: %s\n", e.what());
  }*/
  return 0;
}
}  // namespace yw::master_bot

int main() {
  yw::master_bot::initialize_logging();
  AUTOMEASURE_MB

  yw::master_bot::bot_example();

  return EXIT_SUCCESS;
}
