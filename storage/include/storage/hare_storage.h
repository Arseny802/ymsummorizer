#pragma once

namespace ymsummorizer::storage {
  void initialize_logging();
  hare::hlogger_ptr log();
} // namespace ymsummorizer::storage

#define GAZETA_STORAGE_PROJECT_NAME "ymsummorizer"
#define GAZETA_STORAGE_MODULE_NAME "storage"
#define GAZETA_STORAGE_LOGGER_NAME GAZETA_STORAGE_PROJECT_NAME "." GAZETA_STORAGE_MODULE_NAME

#define AUTOLOG_ST AUTOLOG_IN(GAZETA_STORAGE_LOGGER_NAME);
#define AUTOMEASURE_ST AUTOMEASURE_IN(GAZETA_STORAGE_LOGGER_NAME);
