#pragma once

namespace ymsummorizer::storage {
  void initialize_logging();
  hare::hlogger_ptr log();
} // namespace ymsummorizer::storage

#define YMSUMMORIZER_STORAGE_PROJECT_NAME "ymsummorizer"
#define YMSUMMORIZER_STORAGE_MODULE_NAME "storage"
#define YMSUMMORIZER_STORAGE_LOGGER_NAME YMSUMMORIZER_STORAGE_PROJECT_NAME "." YMSUMMORIZER_STORAGE_MODULE_NAME

#define AUTOLOG_ST AUTOLOG_IN(YMSUMMORIZER_STORAGE_LOGGER_NAME);
#define AUTOMEASURE_ST AUTOMEASURE_IN(YMSUMMORIZER_STORAGE_LOGGER_NAME);
