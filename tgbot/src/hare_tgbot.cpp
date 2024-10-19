#include "pch.h"
#include "hare/config_custom.h"

namespace ymsummorizer::tgbot {
  namespace {
    std::string common_logger_name;

    void initialize_logging_client() {
      auto config = std::make_unique<hare::config_custom>();

      config->set_project_name("ymsummorizer");
      config->set_module_name("tgbot");
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
  }

  hare::hlogger_ptr log() {
    return hare::get_logger(common_logger_name);
  }
} // namespace ymsummorizer::tgbot
