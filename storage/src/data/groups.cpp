#include "groups.h"
#include "hare/defs.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>

namespace ymsummorizer::storage::data {

group_generator::group_generator(db::manager_base_ptr db): db_(std::move(db)) {
  AUTOTRACE;
}

group_generator::~group_generator() {
  AUTOTRACE;
}

bool group_generator::generate_groups() {
  try {
    // Сначала генерируем пользователей, нужных для группы
    // Получаем уже существующих пользователей из базы
    auto users = db_->get_stored_users();
    if (users.empty()) {
      log()->error("No users available to assign to a group.");
      return false;
    }

    common::group group;
    group.id = boost::uuids::to_string(boost::uuids::random_generator()());
    group.name = "DefaultGoup";
    for (const auto& user: users) {
      group.user_ids.push_back(user.id);
    }

    if (!db_->add_group(group)) {
      log()->error("Failed to add generated group to database.");
      return false;
    }

    return true;
  } catch (const std::exception& e) {
    log()->error("Error during group generation: {0}", e.what());
    return false;
  }
}

}  // namespace ymsummorizer::storage::data
