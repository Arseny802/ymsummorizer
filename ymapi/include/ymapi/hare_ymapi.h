#pragma once
#include "hare/config_custom.h"
#include "hare/hare.hpp"

namespace ymsummorizer::ymapi {
  void initialize_logging();
  hare::hlogger_ptr log();
} // namespace ymsummorizer::ymapi

#define YMSUMMORIZER_YMAPI_PROJECT_NAME "ymsummorizer"
#define YMSUMMORIZER_YMAPI_MODULE_NAME "ymapi"
#define YMSUMMORIZER_YMAPI_LOGGER_NAME YMSUMMORIZER_YMAPI_PROJECT_NAME "." YMSUMMORIZER_YMAPI_MODULE_NAME

#define AUTOLOG_ST AUTOLOG_IN(YMSUMMORIZER_YMAPI_LOGGER_NAME);
#define AUTOMEASURE_ST AUTOMEASURE_IN(YMSUMMORIZER_YMAPI_LOGGER_NAME);
