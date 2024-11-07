#pragma once
#include "storage/db_manager.h"
#include "tgbot/command_type.h"

#include <memory>

#include <tgbot/tgbot.h>

namespace ymsummorizer::tgbot::helpers {

  class user_autorization {
public:
    // user_autorization(std::string token, storage::db_manager& db);
    user_autorization();
    //user_autorization(storage::db_manager& db);
    virtual ~user_autorization();

    user_autorization(const user_autorization&) = delete;
    user_autorization(user_autorization&&) = delete;
    user_autorization operator=(const user_autorization&) = delete;
    user_autorization operator=(user_autorization&&) = delete;

protected:
    bool validate_user(TgBot::Message::Ptr message);

    template<command_type CT>
    bool validate_user_command(TgBot::Message::Ptr message);

private:
    //storage::db_manager& db_;
  };

  template<>
  bool user_autorization::validate_user_command<command_type::start>(TgBot::Message::Ptr message);
  template<>
  bool user_autorization::validate_user_command<command_type::user_token_add>(TgBot::Message::Ptr message);
  template<>
  bool user_autorization::validate_user_command<command_type::user_token_erase>(TgBot::Message::Ptr message);
  template<>
  bool user_autorization::validate_user_command<command_type::user_view>(TgBot::Message::Ptr message);

  template<>
  bool user_autorization::validate_user_command<command_type::group_list>(TgBot::Message::Ptr message);

  template<>
  bool user_autorization::validate_user_command<command_type::group_create>(TgBot::Message::Ptr message);

  template<>
  bool user_autorization::validate_user_command<command_type::group_delete>(TgBot::Message::Ptr message);

  template<>
  bool user_autorization::validate_user_command<command_type::group_user_add>(TgBot::Message::Ptr message);
  template<>
  bool user_autorization::validate_user_command<command_type::group_user_remove>(TgBot::Message::Ptr message);
  template<>
  bool user_autorization::validate_user_command<command_type::group_leave>(TgBot::Message::Ptr message);

  template<>
  bool user_autorization::validate_user_command<command_type::group_playslit_list>(TgBot::Message::Ptr message);
  template<>
  bool user_autorization::validate_user_command<command_type::group_playslit_view>(TgBot::Message::Ptr message);
  template<>
  bool user_autorization::validate_user_command<command_type::group_playslit_add>(TgBot::Message::Ptr message);
  template<>
  bool user_autorization::validate_user_command<command_type::group_playslit_remove>(TgBot::Message::Ptr message);

} // namespace ymsummorizer::tgbot::helpers
