#pragma once

namespace ymsummorizer::service {
  void initialize_logging();
  hare::hlogger_ptr log();
} // namespace ymsummorizer::service

#define YMSUMMORIZER_SERVICE_PROJECT_NAME "ymsummorizer"
#define YMSUMMORIZER_SERVICE_MODULE_NAME "service"
#define YMSUMMORIZER_SERVICE_LOGGER_NAME YMSUMMORIZER_SERVICE_PROJECT_NAME "." YMSUMMORIZER_SERVICE_MODULE_NAME

#define AUTOLOG_STE AUTOLOG_IN(YMSUMMORIZER_SERVICE_LOGGER_NAME);
#define AUTOMEASURE_STE AUTOMEASURE_IN(YMSUMMORIZER_SERVICE_LOGGER_NAME);
