#pragma once
#include "command_type.h"
#include <cstdint>
#include <ostream>

namespace ymsummorizer::tgbot {
  enum class access_rights_haha {
    membership,
    adminship,
    group_create,
    group_user_edit,
    group_playlist_edit,

  };

  struct access_rights {
    using mask_t = uint64_t;
    mask_t rights = 0;

    void add(command_type cmd) { rights |= static_cast<mask_t>(cmd); }

    void remove(command_type cmd) { rights &= ~static_cast<mask_t>(cmd); }

    bool contains(command_type cmd) const { return rights & static_cast<mask_t>(cmd); }

private:
    friend std::ostream& operator<<(std::ostream& os, const access_rights& ar) {
      os << "Rights: " << std::hex << ar.rights;
      return os;
    }

    static constexpr mask_t default_mask = 0;
  };

  enum class group_permissions { none = 0, user_edit, playlist_edit };
} // namespace ymsummorizer::tgbot