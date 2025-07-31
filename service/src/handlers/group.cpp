#include "pch.h"

#include "group.h"

namespace ymsummorizer::service::handlers {

group::group(storage::db_manager& db) : db_(db) {
}
group::~group() = default;

tgbot::ymsummorizer_callback_result::ptr group::on_group_list(const tgbot::user_interaction&) {
  AUTOTRACE
  auto ret = std::make_shared<tgbot::group_list_callback_result>();
  try {
    ret.get()->groups = db_.get_stored_groups();
    auto users = db_.get_stored_users();

    for (auto& group : ret.get()->groups) {
      for (auto& user_ids : group.user_ids) {
        auto it = std::find_if(users.begin(), users.end(), [&user_ids](const common::user& u) {
          return u.id == user_ids;
        });
        if (it == users.end()) {
          continue;
        }
        ret.get()->filtered_users.emplace(user_ids, std::move(*it));
      }
    }

    ret->ok = true;
  } catch (const std::exception& e) {
    ret->ok = false;
    ret->error_msg = e.what();
  }
  return ret;
}

tgbot::ymsummorizer_callback_result::ptr group::on_group_create(
    const tgbot::user_interaction& interaction) {
  AUTOTRACE
  auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
  const auto found_group_name_iter =
      interaction.arguments.find(tgbot::user_interaction::key_group_name);
  if (found_group_name_iter == interaction.arguments.cend()) {
    log()->warning("Message in interaction for group_create is empty.");
    ret->ok = false;
    return ret;
  }

  try {
    common::group group;
    group.id = boost::uuids::to_string(boost::uuids::random_generator()());
    group.name = found_group_name_iter->second;
    db_.add_group(group);
    ret->ok = true;
    log()->info("Group '{}' created.", group.name);
  } catch (const std::exception& e) {
    ret->ok = false;
    ret->error_msg = e.what();
  }

  return ret;
}
tgbot::ymsummorizer_callback_result::ptr group::on_group_delete(const tgbot::user_interaction& ui) {
  AUTOTRACE
  auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
  ret->ok = true;
  return ret;
}

}  // namespace ymsummorizer::service::handlers
