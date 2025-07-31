#pragma once
#include "storage/db_manager.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

namespace ymsummorizer::service::handlers {

class group_user {
 public:
  group_user(storage::db_manager& db);
  virtual ~group_user();

  group_user(const group_user&) = delete;
  group_user& operator=(const group_user&) = delete;

  group_user(group_user&&) = delete;
  group_user& operator=(group_user&&) = delete;

  tgbot::ymsummorizer_callback_result::ptr on_group_user_add(const tgbot::user_interaction& ui);
  tgbot::ymsummorizer_callback_result::ptr on_group_user_remove(const tgbot::user_interaction& ui);
  tgbot::ymsummorizer_callback_result::ptr on_group_leave(const tgbot::user_interaction& ui);

 private:
  storage::db_manager& db_;
};

}  // namespace ymsummorizer::service::handlers
