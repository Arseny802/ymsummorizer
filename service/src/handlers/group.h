#pragma once
#include "storage/db_manager.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

namespace ymsummorizer::service::handlers {

  class group {
public:
    group(storage::db_manager& db);
    virtual ~group();

    group(const group&) = delete;
    group& operator=(const group&) = delete;

    group(group&&) = delete;
    group& operator=(group&&) = delete;

    tgbot::ymsummorizer_callback_result::ptr on_group_list(const tgbot::user_interaction& ui);
    tgbot::ymsummorizer_callback_result::ptr on_group_create(const tgbot::user_interaction& ui);
    tgbot::ymsummorizer_callback_result::ptr on_group_delete(const tgbot::user_interaction& ui);

private:
    storage::db_manager& db_;
  };

} // namespace ymsummorizer::service::handlers
