#pragma once
#include "storage/db_manager.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

#include <map>
#include <memory>
#include <queue>

// #include <tgbot/tgbot.h>
#include <tgbot/types/Message.h>

namespace ymsummorizer::tgbot::helpers {

  class usrcmd_callbck_cache final {
public:
    using ptr = std::shared_ptr<usrcmd_callbck_cache>;
    using user_cmd = std::function<bool(const TgBot::Message::Ptr& message)>;
    using callback_cmd = std::function<ymsummorizer_callback_result::ptr(const user_interaction&)>;
    using callback_queue = std::queue<user_cmd>;

    usrcmd_callbck_cache();
    ~usrcmd_callbck_cache();

    bool command_queue_contains(const std::string& chat_id) const;
    bool command_queue_erase(const std::string& chat_id);
    bool command_queue_clear(const std::string& chat_id);
    void command_queue_emplace(const std::string& chat_id);
    void command_queue_emplace_next(const std::string& chat_id, user_cmd&& cmd);
    bool try_handle_command(TgBot::Message::Ptr message);

    std::array<callback_cmd, command_type_count> callback_commands;

private:
    std::map<std::string, callback_queue> user_command_queue_;
  };
} // namespace ymsummorizer::tgbot::helpers
