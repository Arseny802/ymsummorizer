#pragma once

namespace ymsummorizer::tgbot {
  void initialize_logging();
  hare::hlogger_ptr log();
} // namespace ymsummorizer::tgbot

#define YMS_TGBOT_PROJECT_NAME "ymsummorizer"
#define YMS_TGBOT_MODULE_NAME "tgbot"
#define YMS_TGBOT_LOGGER_NAME YMS_TGBOT_PROJECT_NAME "." YMS_TGBOT_MODULE_NAME

#define AUTOLOG_TG AUTOLOG_IN(YMS_TGBOT_LOGGER_NAME);
#define AUTOMEASURE_TG AUTOMEASURE_IN(YMS_TGBOT_LOGGER_NAME);
