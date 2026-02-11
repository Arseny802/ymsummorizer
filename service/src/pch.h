#pragma once
#include "common/common_pch.h"

#ifndef PROJECT_NAME
#  define PROJECT_NAME "ymsummorizer"
#endif
#ifndef MODULE_NAME
#  define MODULE_NAME "service"
#endif
#include "hare/hare.hpp"
// Default logger object getter, no config provided.
MAKE_GETTER(log)

#include "storage/db_manager.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_bot.h"
#include "ymapi/client.h"
