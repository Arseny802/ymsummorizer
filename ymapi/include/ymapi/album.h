#include <memory>
#include <string>

namespace ymsummorizer::ymapi {

  struct album {
    uint64_t id;
    uint64_t releaseDate;
    std::string name;

    uint64_t trackCount;
    uint64_t likesCount;

    std::string metaType;
    std::string genre;
    bool available;
    int year;
  };

  using album_ptr = std::shared_ptr<album>;
} // namespace ymsummorizer::ymapi
