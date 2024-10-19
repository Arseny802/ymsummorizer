#pragma once
#include <memory>
#include <string>

namespace ymsummorizer::ymapi {

  struct artist {
    uint64_t id;
    std::string name;
    bool available;
  };

  using artist_ptr = std::shared_ptr<artist>;
} // namespace ymsummorizer::ymapi
