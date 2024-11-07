#pragma once
#include "storage/db_manager.h"
#include "tgbot/command_type.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_callback_result.h"

#include "handlers/group.h"
#include "handlers/group_playlist.h"
#include "handlers/group_user.h"
#include "handlers/user.h"
#include "helpers/user_autorization.h"
#include "helpers/usrcmd_callbck_cache.h"

#include <memory>

#include <tgbot/tgbot.h>

namespace ymsummorizer::tgbot {

  class bot_impl final: public handlers::user<bot_impl>,
                        public handlers::group<bot_impl>,
                        public handlers::group_playlist<bot_impl>,
                        public handlers::group_user<bot_impl> {
public:
    using callback_cmd = std::function<ymsummorizer_callback_result::ptr(const user_interaction&)>;

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
    void set_callback_command(command_type ct, callback_cmd&& callback);

    TgBot::Bot& get_bot() { return bot_; }
    storage::db_manager& get_db() { return db_; }
    helpers::usrcmd_callbck_cache& get_callbck_cache() { return usrcmd_callbck_cache_; }

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
    ymsummorizer_callback_result::ptr run_command_callback(const user_interaction& ui);

    std::string token_;
    TgBot::Bot bot_;
    storage::db_manager& db_;

    std::unordered_map<command_type, callback_cmd> callback_commands_;

    // std::map<std::string, std::function<void(const TgBot::Message::Ptr& message)>> user_command_queue_;
    using callback_queue = std::queue<std::function<bool(const TgBot::Message::Ptr& message)>>;
    std::map<std::string, callback_queue> user_command_queue_;

    helpers::usrcmd_callbck_cache usrcmd_callbck_cache_;
  };

} // namespace ymsummorizer::tgbot
