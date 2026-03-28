#pragma once

namespace ymsummorizer::storage::db {
using version_t = uint16_t;
enum class version : version_t {
  none,
  v0001,
  v0002,
  v0003,
  v0004,
  v0005,
  v0006,
  v0007,
  v0008,
  v0009,
  v0010,
  v0011,
  v0012,
  v0013,
  v0014,
  v0015,
  v0016
};

constexpr version kMaxVersion = version::v0001;

}  // namespace ymsummorizer::storage::db
