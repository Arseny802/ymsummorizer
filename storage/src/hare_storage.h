#pragma once
#ifndef PROJECT_NAME
#define PROJECT_NAME "ymsummorizer"
#endif
#ifndef MODULE_NAME
#define MODULE_NAME "storage"
#endif
#include "hare/hare.hpp"

namespace ymsummorizer::storage {
hare::hlogger_ptr log();
}  // namespace ymsummorizer::storage
