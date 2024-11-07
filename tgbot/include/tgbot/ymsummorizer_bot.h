#pragma once
#include "command_type.h"
#include "storage/db_manager.h"
#include "user_interaction.h"
#include "ymsummorizer_callback_result.h"

#include <functional>
#include <memory>

namespace ymsummorizer::tgbot {
  class bot_impl;

  class ymsummorizer_bot {
public:
    ymsummorizer_bot(storage::db_manager& db);
    ymsummorizer_bot(std::string token, storage::db_manager& db);
    ~ymsummorizer_bot();

    ymsummorizer_bot(const ymsummorizer_bot&) = default;
    ymsummorizer_bot& operator=(const ymsummorizer_bot&) = default;

    ymsummorizer_bot(ymsummorizer_bot&&) = default;
    ymsummorizer_bot& operator=(ymsummorizer_bot&&) = default;

    bool start();
    bool stop();

    bool send_message(const std::string& chat_id, const std::string& text);

    void set_callback_command(command_type ct,
                              std::function<ymsummorizer_callback_result::ptr(const user_interaction&)>&& callback);

private:
    std::unique_ptr<bot_impl> bot_;
  };
} // namespace ymsummorizer::tgbot
