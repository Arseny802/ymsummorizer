#include "pch.h"
#include "group_user.h"

namespace ymsummorizer::service::handlers {

  group_user::group_user(storage::db_manager& db): db_(db) { }
  group_user::~group_user() = default;

  tgbot::ymsummorizer_callback_result::ptr group_user::on_group_user_add(const tgbot::user_interaction& ui) {
    AUTOLOG_STE
    auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
    ret->ok = true;
    return ret;
  }

  tgbot::ymsummorizer_callback_result::ptr group_user::on_group_user_remove(const tgbot::user_interaction& ui) {
    AUTOLOG_STE
    auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
    ret->ok = true;
    return ret;
  }

  tgbot::ymsummorizer_callback_result::ptr group_user::on_group_leave(const tgbot::user_interaction& ui) {
    AUTOLOG_STE
    auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
    ret->ok = true;
    return ret;
  }
} // namespace ymsummorizer::service::handlers
