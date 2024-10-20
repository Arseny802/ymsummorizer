#include "pch.h"
#include "storage/db_manager.h"

namespace ymsummorizer::storage::example {

  void main() {
    db_manager dbm(storage_types::cfg_json, "ymsummorizer.storage.example.json");
    if (dbm.connect()) {
      log()->info("Connected.");
    } else {
      log()->error("Connected.");
    }

    auto users = dbm.get_stored_users();
    for (size_t index = 0; index < users.size(); ++index) {
      log()->info("Got user {}: {}", index, users[index].format());
    }

    auto groups = dbm.get_stored_groups();
    for (size_t index = 0; index < groups.size(); ++index) {
      log()->info("Got group {}: {}", index, groups[index].format());

      for (const auto& user_id: groups[index].user_ids) {
        log()->info("  - user: {}", user_id);
      }
    }

    auto admins = dbm.get_admin_ids();
    for (size_t index = 0; index < admins.size(); ++index) {
      const auto user_iter = std::find_if(
          users.cbegin(), users.cend(), [&id = admins[index]](const common::user& usr) { return usr.id == id; });
      if (user_iter != users.cend()) {
        log()->info("Got admin {}: {} ({})", index, admins[index], user_iter->name);
      } else {
        log()->info("Got admin {}: {} ()", index, admins[index]);
      }
    }

    auto superadmins = dbm.get_superadmin_ids();
    for (size_t index = 0; index < superadmins.size(); ++index) {
      log()->info("Got superadmin {}: {}", index, superadmins[index]);
    }
  }

} // namespace ymsummorizer::storage::example

int main() {
  ymsummorizer::storage::example::initialize_logging();
  ymsummorizer::storage::example::main();

  return EXIT_SUCCESS;
}
