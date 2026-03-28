#include "administrators.h"
#include "hare/defs.h"
#include <nlohmann/json.hpp>

namespace ymsummorizer::storage::data {

administrator_generator::administrator_generator(db::manager_base_ptr db): db_(std::move(db)) {
  AUTOTRACE;
}

administrator_generator::~administrator_generator() {
  AUTOTRACE;
}

bool administrator_generator::generate_administrators() {
  try {
    // Получаем существующих пользователей
    auto users = db_->get_stored_users();
    if (users.size() < 2) {
      log()->error("Not enough users to assign as administrators.");
      return false;
    }

    // Назначаем первого пользователя админом
    if (!db_->add_admin(users[0])) {
      log()->error("Failed to add first user as admin.");
      return false;
    }

    auto groups = db_->get_stored_groups();
    if (groups.size() < 2) {
      log()->error("Not enough groups to assign as superadministrators.");
      return false;
    }
    if (std::find(groups[0].user_ids.cbegin(), groups[0].user_ids.cend(), users[0].id) == groups[0].user_ids.cend()) {
      log()->error("Not enough users to assign as superadministrators.");
      return false;
    }

    // Назначаем первую группу суперадминами
    if (!db_->add_superadmin(groups[0])) {
      log()->error("Failed to add first user as superadmin.");
      return false;
    }

    return true;
  } catch (const std::exception& e) {
    log()->error("Error during administrator generation: {0}", e.what());
    return false;
  }
}

}  // namespace ymsummorizer::storage::data
