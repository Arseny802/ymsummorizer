#include "pch.h"

#include "bot_impl.h"
#include "hare/hare.hpp"

namespace ymsummorizer::service {

bot_impl::bot_impl(storage::db_manager&& db)
    : handlers::user(db_),
      handlers::group(db_),
      handlers::group_playlist(db_),
      handlers::group_user(db_),
      db_(std::move(db)),
      bot_(db_) {
  AUTOTRACEFA

  bot_.set_callback_command(tgbot::command_type::group_list,
                            std::bind(&group::on_group_list, this, std::placeholders::_1));
  bot_.set_callback_command(tgbot::command_type::group_create,
                            std::bind(&group::on_group_create, this, std::placeholders::_1));
  bot_.set_callback_command(tgbot::command_type::group_delete,
                            std::bind(&group::on_group_delete, this, std::placeholders::_1));

  bot_.set_callback_command(tgbot::command_type::group_user_add,
                            std::bind(&group_user::on_group_user_add, this, std::placeholders::_1));
  bot_.set_callback_command(
      tgbot::command_type::group_user_remove,
      std::bind(&group_user::on_group_user_remove, this, std::placeholders::_1));
  bot_.set_callback_command(tgbot::command_type::group_leave,
                            std::bind(&group_user::on_group_leave, this, std::placeholders::_1));

  bot_.set_callback_command(
      tgbot::command_type::group_playslit_list,
      std::bind(&group_playlist::on_group_playslit_list, this, std::placeholders::_1));
  bot_.set_callback_command(
      tgbot::command_type::group_playslit_view,
      std::bind(&group_playlist::on_group_playslit_view, this, std::placeholders::_1));
  bot_.set_callback_command(
      tgbot::command_type::group_playslit_add,
      std::bind(&group_playlist::on_group_playslit_add, this, std::placeholders::_1));
  bot_.set_callback_command(
      tgbot::command_type::group_playslit_remove,
      std::bind(&group_playlist::on_group_playslit_remove, this, std::placeholders::_1));

  bot_.set_callback_command(tgbot::command_type::start,
                            std::bind(&user::on_start, this, std::placeholders::_1));
  bot_.set_callback_command(tgbot::command_type::user_token_add,
                            std::bind(&user::on_user_token_add, this, std::placeholders::_1));
  bot_.set_callback_command(tgbot::command_type::user_token_erase,
                            std::bind(&user::on_user_token_erase, this, std::placeholders::_1));
  // bot_.set_callback_command(tgbot::command_type::token_edit,
  //                           std::bind(&user::on_token_edit, this, std::placeholders::_1));
  bot_.set_callback_command(tgbot::command_type::user_view,
                            std::bind(&user::on_user_view, this, std::placeholders::_1));
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
}  // namespace ymsummorizer::service
