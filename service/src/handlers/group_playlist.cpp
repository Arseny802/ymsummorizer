#include "pch.h"
#include "group_playlist.h"
#include "ymapi/client.h"

namespace ymsummorizer::service::handlers {

  group_playlist::group_playlist(storage::db_manager& db): db_(db) { }
  group_playlist::~group_playlist() = default;

  tgbot::ymsummorizer_callback_result::ptr group_playlist::on_group_playslit_list(const tgbot::user_interaction& ui) {
    AUTOLOG_STE
    auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
    ret->ok = true;
    return ret;
  }

  tgbot::ymsummorizer_callback_result::ptr group_playlist::on_group_playslit_view(const tgbot::user_interaction& ui) {
    AUTOLOG_STE
    auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
    ret->ok = true;
    return ret;
  }

  tgbot::ymsummorizer_callback_result::ptr group_playlist::on_group_playslit_add(
      const tgbot::user_interaction& interaction) {
    AUTOLOG_STE
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
        const auto user_it = std::find_if(
            stored_users.cbegin(), stored_users.cend(), [&user_id](const common::user& u) { return u.id == user_id; });

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

  tgbot::ymsummorizer_callback_result::ptr group_playlist::on_group_playslit_remove(const tgbot::user_interaction& ui) {
    AUTOLOG_STE
    auto ret = std::make_shared<tgbot::ymsummorizer_callback_result>();
    ret->ok = true;
    return ret;
  }

} // namespace ymsummorizer::service::handlers
