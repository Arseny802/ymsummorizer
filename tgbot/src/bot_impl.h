#pragma once
#include "helpers/user_autorization.h"
#include "storage/db_manager.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

#include <memory>

#include <tgbot/tgbot.h>

namespace ymsummorizer::tgbot {

  class bot_impl final: private helpers::user_autorization {
public:
    bot_impl(std::string token, storage::db_manager& db);
    ~bot_impl();

    bot_impl() = delete;
    bot_impl(const bot_impl&) = delete;
    bot_impl(bot_impl&&) = delete;
    bot_impl operator=(const bot_impl&) = delete;
    bot_impl operator=(bot_impl&&) = delete;

    bool start();
    bool stop();

    bool send_message(const std::string& chat_id, const std::string& text);
    void set_callback_command(command_type ct,
                              std::function<ymsummorizer_callback_result::ptr(const user_interaction&)>&& callback);

private:
    void on_start(TgBot::Message::Ptr message);
    void on_user_view(TgBot::Message::Ptr message);
    void on_user_token_add(TgBot::Message::Ptr message);
    void on_user_token_erase(TgBot::Message::Ptr message);

    void on_group_list(TgBot::Message::Ptr message);
    void on_group_leave(TgBot::Message::Ptr message);
    void on_group_playslit_list(TgBot::Message::Ptr message);
    void on_group_playslit_view(TgBot::Message::Ptr message);

    void on_group_user_add(TgBot::Message::Ptr message);
    void on_group_user_remove(TgBot::Message::Ptr message);
    void on_group_playslit_add(TgBot::Message::Ptr message);
    void on_group_playslit_remove(TgBot::Message::Ptr message);

    void on_group_create(TgBot::Message::Ptr message);
    void on_group_delete(TgBot::Message::Ptr message);

    void on_non_command_message(TgBot::Message::Ptr message);

    void init_commands();

    template<command_type CT>
    ymsummorizer_callback_result::ptr run_command_callback(const TgBot::Message::Ptr& message,
                                                           bool use_text = false,
                                                           bool use_answer = false);

    storage::db_manager& get_db() const noexcept override;

    std::string token_;
    TgBot::Bot bot_;
    storage::db_manager& db_;

    std::unordered_map<command_type, std::function<ymsummorizer_callback_result::ptr(const user_interaction&)>>
        callback_commands_;
    std::map<std::string, std::function<void(const TgBot::Message::Ptr& message)>> user_command_queue_;
  };

} // namespace ymsummorizer::tgbot
