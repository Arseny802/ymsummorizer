#include "pch.h"
#include "user.h"

namespace ymsummorizer::service::handlers {

  user::user(storage::db_manager& db): db_(db) { }
  user::~user() = default;

  tgbot::ymsummorizer_callback_result::ptr user::on_start(const tgbot::user_interaction& ui) {
    AUTOLOG_STE
    auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
    ret->ok = true;
    return ret;
  }

  tgbot::ymsummorizer_callback_result::ptr user::on_user_token_add(const tgbot::user_interaction& ui) {
    AUTOLOG_STE
    auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
    ret->ok = true;
    return ret;
  }

  tgbot::ymsummorizer_callback_result::ptr user::on_user_token_erase(const tgbot::user_interaction& ui) {
    AUTOLOG_STE
    auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
    ret->ok = true;
    return ret;
  }

  tgbot::ymsummorizer_callback_result::ptr user::on_token_edit(const tgbot::user_interaction& ui) {
    AUTOLOG_STE
    auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
    ret->ok = true;
    return ret;
  }

  tgbot::ymsummorizer_callback_result::ptr user::on_user_view(const tgbot::user_interaction& ui) {
    AUTOLOG_STE
    auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
    ret->ok = true;
    return ret;
  }

} // namespace ymsummorizer::service::handlers
