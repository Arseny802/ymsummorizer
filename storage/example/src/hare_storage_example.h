#pragma once

namespace ymsummorizer::storage::example {
  void initialize_logging();
  hare::hlogger_ptr log();
} // namespace ymsummorizer::storage::example

#define GAZETA_STORAGE_EXAMPLE_PROJECT_NAME "ymsummorizer"
#define GAZETA_STORAGE_EXAMPLE_MODULE_NAME "storage.example"
#define GAZETA_STORAGE_EXAMPLE_LOGGER_NAME GAZETA_STORAGE_EXAMPLE_PROJECT_NAME "." GAZETA_STORAGE_EXAMPLE_MODULE_NAME

#define AUTOLOG_STE AUTOLOG_IN(GAZETA_STORAGE_EXAMPLE_LOGGER_NAME);
#define AUTOMEASURE_STE AUTOMEASURE_IN(GAZETA_STORAGE_EXAMPLE_LOGGER_NAME);
