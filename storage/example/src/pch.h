#pragma once
#include "common/common_pch.h"

#ifndef PROJECT_NAME
#  define PROJECT_NAME "ymsummorizer"
#endif
#ifndef MODULE_NAME
#  define MODULE_NAME "storage.example"
#endif
#include <hare/hare.hpp>
namespace ymsummorizer::storage::example {
MAKE_GETTER(log)
}  // namespace ymsummorizer::storage::example
