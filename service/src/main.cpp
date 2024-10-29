#include "pch.h"
#include "storage/db_manager.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_bot.h"

namespace ymsummorizer::service {

  class main_impl {
public:
    main_impl(): db_(storage::storage_types::cfg_json, "ymsummorizer.storage.json"), bot_(db_) {
      db_.connect();
      bot_.set_callback_command(tgbot::command_type::group_create,
                                std::bind(&main_impl::on_group_create, this, std::placeholders::_1));
      bot_.set_callback_command(tgbot::command_type::group_list,
                                std::bind(&main_impl::on_group_list, this, std::placeholders::_1));
    }

    tgbot::ymsummorizer_callback_result::ptr on_group_create(const tgbot::user_interaction& interaction) {
      auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
      if (!interaction.message.has_value()) {
        log()->warning("Message in interaction for group_create is empty.");
        ret->ok = false;
        return ret;
      }

      try {
        common::group group;
        group.id = boost::uuids::to_string(boost::uuids::random_generator()());
        group.name = interaction.message.value();
        db_.add_group(group);
        ret->ok = true;
        log()->info("Group '{}' created.", group.name);
      } catch (const std::exception& e) {
        ret->ok = false;
        ret->error_msg = e.what();
      }

      return ret;
    }

    tgbot::ymsummorizer_callback_result::ptr on_group_list(const tgbot::user_interaction&) {
      auto ret = std::make_shared<tgbot::group_list_callback_result>();
      try {
        ret.get()->groups = db_.get_stored_groups();
        auto users = db_.get_stored_users();

        for (auto& group: ret.get()->groups) {
          for (auto& user_ids: group.user_ids) {
            auto it = std::find_if(
                users.begin(), users.end(), [&user_ids](const common::user& u) { return u.id == user_ids; });
            if (it == users.end()) {
              continue;
            }
            ret.get()->filtered_users.emplace(user_ids, std::move(*it));
          }
        }

        /*
                ret.get()->filtered_users.erase(std::remove_if(
                    ret.get()->filtered_users.begin(),
                    ret.get()->filtered_users.end(),
                    [&groups = ret.get()->groups](const common::user& user) {
                      return std::none_of(groups.cbegin(), groups.cend(), [&user](const common::group& group) {
                        return std::find(group.user_ids.cbegin(), group.user_ids.cend(), user.id) !=
           group.user_ids.cend();
                      });
                    }));
                    */

        ret->ok = true;
      } catch (const std::exception& e) {
        ret->ok = false;
        ret->error_msg = e.what();
      }
      return ret;
    }

    void main() {
      log()->debug("Starting...");

      if (!bot_.start()) {
        log()->error("Failed to start bot");
        return;
      }

      log()->info("Started");

      constexpr size_t counter_step = 5;
      size_t counter = 1;
      while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(counter_step));
        log()->debug("Already running for {} second(s).", (counter_step * counter++));
      }
    }

private:
    storage::db_manager db_;
    tgbot::ymsummorizer_bot bot_;
  };

} // namespace ymsummorizer::service

int main() {
  ymsummorizer::service::initialize_logging();
  ymsummorizer::service::main_impl main_impl;

  main_impl.main();

  return EXIT_SUCCESS;
}
