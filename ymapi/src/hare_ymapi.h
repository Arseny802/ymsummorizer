#pragma once
#ifndef PROJECT_NAME
#define PROJECT_NAME "ymsummorizer"
#endif
#ifndef MODULE_NAME
#define MODULE_NAME "ymapi"
#endif
#include "hare/hare.hpp"

namespace ymsummorizer::ymapi {
hare::hlogger_ptr log();
}  // namespace ymsummorizer::ymapi
