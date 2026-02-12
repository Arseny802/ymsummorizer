#pragma once

// Local files
#include "common/common_pch.h"

#ifndef PROJECT_NAME
#  define PROJECT_NAME "ymsummorizer"
#endif
#ifndef MODULE_NAME
#  define MODULE_NAME "tgbot"
#endif
#include <hare/hare.hpp>
namespace ymsummorizer::tgbot {
MAKE_GETTER(log)
}  // namespace ymsummorizer::tgbot

// Standard libraries
#include <fmt/format.h>

#include <boost/algorithm/string.hpp>
