#include "pch.h"
#include "hare_service.h"
#include "storage/hare_storage.h"
#include "tgbot/hare_tgbot.h"
#include "ymapi/hare_ymapi.h"

namespace ymsummorizer::service {
  namespace {
    std::string common_logger_name;

    void initialize_logging_client() {
      auto config = std::make_unique<hare::config_custom>();

      config->set_project_name(YMSUMMORIZER_SERVICE_PROJECT_NAME);
      config->set_module_name(YMSUMMORIZER_SERVICE_MODULE_NAME);
      config->set_hlevel(hare::hlevels::trace);

      hare::sinks_info sinks_info;
      sinks_info.fs.truncate = false;

      config->set_sinks_info(std::move(sinks_info));

      common_logger_name = config->get_logger_name();
      hare::register_logger(std::move(config));
    }
  } // namespace

  void initialize_logging() {
    initialize_logging_client();
    ymsummorizer::storage::initialize_logging();
    ymsummorizer::tgbot::initialize_logging();
    ymsummorizer::ymapi::initialize_logging();
  }

  hare::hlogger_ptr log() {
    return hare::get_logger(common_logger_name);
  }
} // namespace ymsummorizer::service
