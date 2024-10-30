#include "pch.h"
#include "storage/db_manager.h"
#include "tgbot/user_interaction.h"
#include "tgbot/ymsummorizer_bot.h"
#include "ymapi/client.h"

namespace ymsummorizer::service {

  class main_impl {
public:
    main_impl(): db_(storage::storage_types::cfg_json, "ymsummorizer.storage.json"), bot_(db_) {
      db_.connect();
      bot_.set_callback_command(tgbot::command_type::group_create,
                                std::bind(&main_impl::on_group_create, this, std::placeholders::_1));
      bot_.set_callback_command(tgbot::command_type::group_list,
                                std::bind(&main_impl::on_group_list, this, std::placeholders::_1));
      bot_.set_callback_command(tgbot::command_type::group_playslit_add,
                                std::bind(&main_impl::on_group_playslit_add, this, std::placeholders::_1));

      // ymapi_client_.reserve(db_.get_stored_users().size());
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

        ret->ok = true;
      } catch (const std::exception& e) {
        ret->ok = false;
        ret->error_msg = e.what();
      }
      return ret;
    }

    tgbot::ymsummorizer_callback_result::ptr on_group_create(const tgbot::user_interaction& interaction) {
      auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
      const auto found_group_name_iter = interaction.arguments.find(tgbot::user_interaction::key_group_name);
      if (found_group_name_iter == interaction.arguments.cend()) {
        log()->warning("Message in interaction for group_create is empty.");
        ret->ok = false;
        return ret;
      }

      try {
        common::group group;
        group.id = boost::uuids::to_string(boost::uuids::random_generator()());
        group.name = found_group_name_iter->second;
        db_.add_group(group);
        ret->ok = true;
        log()->info("Group '{}' created.", group.name);
      } catch (const std::exception& e) {
        ret->ok = false;
        ret->error_msg = e.what();
      }

      return ret;
    }

    tgbot::ymsummorizer_callback_result::ptr on_group_playslit_add(const tgbot::user_interaction& interaction) {
      auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();

      const auto found_group_name_iter = interaction.arguments.find(tgbot::user_interaction::key_group_name);
      if (found_group_name_iter == interaction.arguments.cend()) {
        log()->warning("Message in interaction for group_create is empty.");
        ret->ok = false;
        return ret;
      }

      const auto found_playlist_name_iter = interaction.arguments.find(tgbot::user_interaction::key_playlist_name);
      if (found_playlist_name_iter == interaction.arguments.cend()) {
        log()->warning("Message in interaction for group_create is empty.");
        ret->ok = false;
        return ret;
      }

      try {
        common::playlist playlist;
        playlist.id = boost::uuids::to_string(boost::uuids::random_generator()());
        const std::string& group_name = found_group_name_iter->second;
        playlist.group_id = found_group_name_iter->second;
        playlist.name = found_playlist_name_iter->second;

        // db_.add_playlist(playlist);
        const auto groups = db_.get_stored_groups();
        auto it = std::find_if(
            groups.begin(), groups.end(), [&group_name](const common::group& g) { return g.name == group_name; });

        if (it == groups.end()) {
          log()->warning("There is no group with name '{}'.", group_name);
          ret->ok = false;
          return ret;
        }

        const auto stored_users = db_.get_stored_users();
        for (const auto& user_id: it->user_ids) {
          const auto user_it = std::find_if(stored_users.cbegin(),
                                            stored_users.cend(),
                                            [&user_id](const common::user& u) { return u.id == user_id; });

          if (user_it == stored_users.cend()) {
            log()->error("There is no user with id '{}'.", user_id);
            continue;
          }

          ymapi::client client(user_it->login_yandex, user_it->token);
          ymapi::playlist_ptr playlist_ym = client.playlist_create(playlist.name);

          if (!playlist_ym) {
            log()->error("Failed to create playlist '{}' for user '{}'.", playlist.name, user_it->login_yandex);
            continue;
          }

          log()->info("Created playlist '{}' (kind {}, uid {}, uuid {}) for user '{}'.",
                      playlist_ym->title,
                      playlist_ym->kind,
                      playlist_ym->uid,
                      playlist_ym->playlistUuid,
                      playlist_ym->owner.login);
        }

        ret->ok = true;
        // log()->info("Group '{}' created.", playlist.name);
      } catch (const std::exception& e) {
        log()->error("Failed to create playlist: {}", e.what());
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
    // std::vector<ymapi::client> ymapi_client_;
  };

} // namespace ymsummorizer::service

int main() {
  ymsummorizer::service::initialize_logging();
  ymsummorizer::service::main_impl main_impl;

  main_impl.main();

  return EXIT_SUCCESS;
}
