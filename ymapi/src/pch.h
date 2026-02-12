#pragma once

// Standard libraries
#include "common/common_pch.h"

// Local files

#ifndef PROJECT_NAME
#  define PROJECT_NAME "ymsummorizer"
#endif
#ifndef MODULE_NAME
#  define MODULE_NAME "ymapi"
#endif
#include <hare/hare.hpp>
namespace ymsummorizer::ymapi {
MAKE_GETTER(log)
}  // namespace ymsummorizer::ymapi

// External Libraries
#include <cpr/cpr.h>

#include <nlohmann/json.hpp>
