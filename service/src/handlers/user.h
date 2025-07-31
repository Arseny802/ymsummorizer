#pragma once
#include "storage/db_manager.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

namespace ymsummorizer::service::handlers {

class user {
 public:
  user(storage::db_manager& db);
  virtual ~user();

  user(const user&) = delete;
  user& operator=(const user&) = delete;

  user(user&&) = delete;
  user& operator=(user&&) = delete;

  tgbot::ymsummorizer_callback_result::ptr on_start(const tgbot::user_interaction& ui);
  tgbot::ymsummorizer_callback_result::ptr on_user_token_add(const tgbot::user_interaction& ui);
  tgbot::ymsummorizer_callback_result::ptr on_user_token_erase(const tgbot::user_interaction& ui);
  tgbot::ymsummorizer_callback_result::ptr on_token_edit(const tgbot::user_interaction& ui);
  tgbot::ymsummorizer_callback_result::ptr on_user_view(const tgbot::user_interaction& ui);

 private:
  storage::db_manager& db_;
};

}  // namespace ymsummorizer::service::handlers
