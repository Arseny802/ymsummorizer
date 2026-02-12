#pragma once
#include "common/common_pch.h"

#ifndef PROJECT_NAME
#  define PROJECT_NAME "ymsummorizer"
#endif
#ifndef MODULE_NAME
#  define MODULE_NAME "service"
#endif
#include <hare/hare.hpp>
namespace ymsummorizer::service {
MAKE_GETTER(log)
}  // namespace ymsummorizer::service
