#include "pch.h"

#include "usrcmd_callbck_cache.h"

namespace ymsummorizer::tgbot::helpers {

ymsummorizer_callback_result::ptr callback_stub(const user_interaction&) {
  log()->error("Callback for command not inited!");
  return nullptr;
}

usrcmd_callbck_cache::usrcmd_callbck_cache() {
  for (size_t i = 0; i < callback_commands.size(); i++) {
    callback_commands[i] = &callback_stub;
  }
}
usrcmd_callbck_cache::~usrcmd_callbck_cache() = default;

bool usrcmd_callbck_cache::command_queue_contains(const std::string& chat_id) const {
  return user_command_queue_.contains(chat_id);
}

bool usrcmd_callbck_cache::command_queue_erase(const std::string& chat_id) {
  return user_command_queue_.erase(chat_id);
}

bool usrcmd_callbck_cache::command_queue_clear(const std::string& chat_id) {
  bool res = command_queue_contains(chat_id);
  command_queue_emplace(chat_id);
  return res;
}

void usrcmd_callbck_cache::command_queue_emplace(const std::string& chat_id) {
  user_command_queue_[chat_id] = std::queue<user_cmd>();
}

void usrcmd_callbck_cache::command_queue_emplace_next(const std::string& chat_id, user_cmd&& cmd) {
  user_command_queue_[chat_id].emplace(cmd);
}

bool usrcmd_callbck_cache::try_handle_command(TgBot::Message::Ptr message) {
  AUTOMEASURE

  auto command_iter = user_command_queue_.find(message->from->username);
  if (command_iter == user_command_queue_.end()) {
    return false;
  }

  log()->info("User {} wrote '{}'. Passing to user_command_queue_",
              message->from->username,
              message->text);
  auto& cmd = command_iter->second.front();
  if (!cmd(message)) {
    user_command_queue_.erase(command_iter);
    log()->warning("Command failed. User {} command queue is empty.", message->from->username);
    return false;
  }

  command_iter->second.pop();
  if (command_iter->second.empty()) {
    user_command_queue_.erase(command_iter);
    log()->info("User {} command queue is empty.", message->from->username);
  }

  return true;
}
}  // namespace ymsummorizer::tgbot::helpers
