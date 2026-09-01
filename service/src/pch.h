#pragma once
#include <gpch.h>

#include <boost/uuid/uuid.hpp>             // uuid class
#include <boost/uuid/uuid_generators.hpp>  // generators
#include <boost/uuid/uuid_io.hpp>          // streaming operators etc.

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
