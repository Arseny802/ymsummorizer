#include "pch.h"

#include "bot_impl.h"

namespace ymsummorizer::service {

bot_impl::bot_impl(storage::db_manager&& db)
    : handlers::user(db_),
      handlers::group(db_),
      handlers::group_playlist(db_),
      handlers::group_user(db_),
      db_(std::move(db)),
      bot_(db_) {
  AUTOTRACEFA

  connect_handler(tgbot::command_type::group_list, &group::on_group_list);
  connect_handler(tgbot::command_type::group_create, &group::on_group_create);
  connect_handler(tgbot::command_type::group_delete, &group::on_group_delete);

  connect_handler(tgbot::command_type::group_user_add, &group_user::on_group_user_add);
  connect_handler(tgbot::command_type::group_user_remove, &group_user::on_group_user_remove);
  connect_handler(tgbot::command_type::group_leave, &group_user::on_group_leave);

  connect_handler(tgbot::command_type::group_playslit_list, &group_playlist::on_group_playslit_list);
  connect_handler(tgbot::command_type::group_playslit_view, &group_playlist::on_group_playslit_view);
  connect_handler(tgbot::command_type::group_playslit_add, &group_playlist::on_group_playslit_add);
  connect_handler(tgbot::command_type::group_playslit_remove, &group_playlist::on_group_playslit_remove);

  connect_handler(tgbot::command_type::start, &user::on_start);
  connect_handler(tgbot::command_type::user_token_add, &user::on_user_token_add);
  connect_handler(tgbot::command_type::user_token_erase, &user::on_user_token_erase);
  // connect(tgbot::command_type::token_edit, &user::on_token_edit);
  connect_handler(tgbot::command_type::user_view, &user::on_user_view);
}

bot_impl::~bot_impl() {
  AUTOTRACE;
}

void bot_impl::main() {
  AUTOTRACE
  log()->debug("Starting...");

  if (!bot_.start()) {
    log()->error("Failed to start bot");
    return;
  }

  log()->info("Started");
  log()->flush();

  constexpr size_t counter_step = 5;
  size_t counter = 1;
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(counter_step));
    log()->debug("Already running for {} second(s).", (counter_step * counter++));
  }
}

template<typename Handler>
void bot_impl::connect_handler(tgbot::command_type cmd, Handler function) {
  bot_.set_callback_command(cmd, std::bind(function, this, std::placeholders::_1));
}
}  // namespace ymsummorizer::service
