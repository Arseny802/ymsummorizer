#pragma once
#ifndef PROJECT_NAME
#define PROJECT_NAME "ymsummorizer"
#endif
#ifndef MODULE_NAME
#define MODULE_NAME "service"
#endif
#include "hare/hare.hpp"

namespace ymsummorizer::service {
void initialize_logging();
hare::hlogger_ptr log();
}  // namespace ymsummorizer::service
