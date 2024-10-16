#include "pch.h"
#include "ymapi/client.h"

#include <format>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

namespace ymsummorizer::ymapi {

  struct invocation_info {
    std::string request_id;
    std::string hostname;
    std::string app_name;
    uint64_t request_duration_millis;
  };

  invocation_info get_invocation_info(nlohmann::json http_response) {
    invocation_info request_info;
    nlohmann::json invocationInfo = http_response["invocationInfo"];
    if (invocationInfo.contains("req-id")) {
      request_info.request_id = invocationInfo["req-id"];
    }
    if (invocationInfo.contains("hostname")) {
      request_info.hostname = invocationInfo["hostname"];
    }
    if (invocationInfo.contains("app-name")) {
      request_info.app_name = invocationInfo["app-name"];
    }
    if (invocationInfo.contains("exec-duration-millis")) {
      request_info.request_duration_millis = invocationInfo["exec-duration-millis"];
    }
    return request_info;
  }

  playlist_ptr setup_playlist(nlohmann::json result) {
    playlist_ptr current_playlist;

    current_playlist->title = result["title"];
    current_playlist->playlistUuid = result["playlistUuid"];
    current_playlist->description = result["description"];
    current_playlist->available = result["available"];
    current_playlist->kind = result["kind"];
    current_playlist->uid = result["uid"];
    current_playlist->revision = result["revision"];
    current_playlist->snapshot = result["snapshot"];
    current_playlist->trackCount = result["trackCount"];
    current_playlist->visibility = result["visibility"] == "public";
    current_playlist->created = result["created"];   // todo
    current_playlist->modified = result["modified"]; // todo
    current_playlist->durationMs = result["durationMs"];
    current_playlist->likesCount = result["likesCount"];

    nlohmann::json owner = result["owner"];
    current_playlist->owner.uid = owner["uid"];
    current_playlist->owner.login = owner["login"];
    current_playlist->owner.name = owner["name"];

    if (result.contains("pager")) {
      nlohmann::json pager = result["owner"];
      current_playlist->pager.total = pager["total"];
      current_playlist->pager.page = pager["page"];
      current_playlist->pager.perPage = pager["perPage"];
    }

    if (result.contains("tracks")) {
      for (const auto& track: result["tracks"]) {
        playlist_track current_track;

        current_track.id = track["id"];
        current_track.originalIndex = track["originalIndex"];
        current_track.timestamp = track["timestamp"];

        nlohmann::json track_info = track["track"];
        current_track.realId = track_info["realId"];
        current_track.title = track_info["title"];
        current_track.version = track_info["version"];

        current_track.available = track_info["available"];
        current_track.durationMs = track_info["durationMs"];

        if (result.contains("artists")) {
          for (const auto& track_artist: track_info["artists"]) {
            artist current_artist;
            current_artist.id = track_artist["id"];
            current_artist.name = track_artist["name"];
            current_artist.available = track_artist["available"];
            current_track.artists.emplace_back(current_artist);
          }
        }

        if (result.contains("albums")) {
          for (const auto& track_album: track_info["albums"]) {
            album current_album;
            current_album.id = track_album["id"];
            current_album.releaseDate = track_album["releaseDate"];
            current_album.name = track_album["name"];
            current_album.trackCount = track_album["trackCount"];
            current_album.likesCount = track_album["likesCount"];
            current_album.metaType = track_album["metaType"];
            current_album.genre = track_album["genre"];
            current_album.releaseDate = track_album["releaseDate"];
            current_album.available = track_album["available"];
            current_album.year = track_album["year"];
            current_track.albums.emplace_back(current_album);
          }
        }

        current_playlist->tracks.emplace_back(std::move(current_track));
      }
    }

    return current_playlist;
  }

  cpr::Header get_base_headers(std::string user_id, std::string token) {
    return {
        {"X-Yandex-Music-Client", user_id},
        {"Authorization", std::format("OAuth {}", token)},
        {"Content-Type", "application/json"},
        {"accept", "application/json"},
    };
  }

  client::client(std::string user_id, std::string token): user_id_(std::move(user_id)), token_(std::move(token)) {
    // trace
  }
  client::~client() {
    // trace
  }

  std::vector<playlist_ptr> client::playlist_list(std::string user_id) {
    cpr::Url Url{std::format("{}/users/{}/playlists/list", host, (user_id.empty() ? user_id_ : user_id))};
    cpr::Parameters parameters;

    cpr::Response response = cpr::Get(Url, parameters, get_base_headers(user_id_, token_));

    std::cout << response.text << std::endl;

    if (response.status_code == 200) {
      nlohmann::json http_response = nlohmann::json::parse(response.text);
      invocation_info request_info = get_invocation_info(http_response);

      std::vector<playlist_ptr> result;
      if (!http_response.contains("result")) {
        return result;
      }

      for (const auto& playlist_info: http_response["result"]) {
        result.emplace_back(setup_playlist(playlist_info));
      }

      return result;
    }

    return {};
  }

  playlist_ptr client::playlist_get_info(int kind, std::string user_id) {
    cpr::Url Url{std::format("{}/users/{}/playlists/{}", host, (user_id.empty() ? user_id_ : user_id), kind)};
    cpr::Parameters parameters;

    cpr::Response response = cpr::Get(Url, parameters, get_base_headers(user_id_, token_));

    std::cout << response.text << std::endl;

    if (response.status_code == 200) {
      nlohmann::json http_response = nlohmann::json::parse(response.text);
      invocation_info request_info = get_invocation_info(http_response);
      return setup_playlist(http_response["result"]);
    }

    return nullptr;
  }

  playlist_ptr client::playlist_create(const std::string& playlist_name, bool visibility) {
    cpr::Url Url{std::format("{}/users/{}/playlists/create", host, user_id_)};
    cpr::Parameters parameters{{"title", playlist_name}, {"visibility", (visibility ? "public" : "private")}};

    cpr::Response response = cpr::Post(Url, parameters, get_base_headers(user_id_, token_));

    std::cout << response.text << std::endl;

    if (response.status_code == 200) {
      nlohmann::json http_response = nlohmann::json::parse(response.text);
      invocation_info request_info = get_invocation_info(http_response);
      return setup_playlist(http_response["result"]);
    }

    return nullptr;
  }

  bool client::playlist_delete(int kind) {
    cpr::Url Url{std::format("{}/users/{}/playlists/{}/delete", host, user_id_, kind)};
    cpr::Parameters parameters;

    cpr::Response response = cpr::Post(Url, parameters, get_base_headers(user_id_, token_));

    std::cout << response.text << std::endl;

    if (response.status_code == 200) {
      nlohmann::json http_response = nlohmann::json::parse(response.text);
      invocation_info request_info = get_invocation_info(http_response);

      std::cout << http_response["result"] << std::endl;
      return true;
    }

    return false;
  }

  playlist_ptr client::playlist_visibility(int kind, bool visibility) {
    cpr::Url Url{std::format("{}/users/{}/playlists/{}/visibility", host, user_id_, kind)};
    cpr::Parameters parameters{{"visibility", (visibility ? "public" : "private")}};

    cpr::Response response = cpr::Post(Url, parameters, get_base_headers(user_id_, token_));

    std::cout << response.text << std::endl;

    if (response.status_code == 200) {
      nlohmann::json http_response = nlohmann::json::parse(response.text);
      invocation_info request_info = get_invocation_info(http_response);
      return setup_playlist(http_response["result"]);
    }

    return nullptr;
  }

  bool client::playlist_track_insert(playlist_ptr playlist, uint64_t track_id) {
    cpr::Url Url{std::format("{}/users/{}/playlists/{}/change-relative", host, user_id_, playlist->kind)};

    nlohmann::json json;
    json["op"] = "insert";
    json["at"] = 0;
    json["tracks"] = nlohmann::json::array();
    json["tracks"].push_back(nlohmann::json::object());
    json["tracks"][0]["id"] = track_id;
    std::cout << json.dump() << std::endl;

    cpr::Parameters parameters{{"diff", json.dump()}, {"revision", std::to_string(playlist->revision)}};
    cpr::Response response = cpr::Post(Url, parameters, get_base_headers(user_id_, token_));
    if (response.status_code == 200) {
      nlohmann::json http_response = nlohmann::json::parse(response.text);
      invocation_info request_info = get_invocation_info(http_response);
      playlist = setup_playlist(http_response["result"]);
      return true;
    }

    return false;
  }

  bool client::playlist_track_delete(playlist_ptr playlist, uint64_t track_id, uint64_t position) {
    cpr::Url Url{std::format("{}/users/{}/playlists/{}/change-relative", host, user_id_, playlist->kind)};

    nlohmann::json json;
    json["op"] = "delete";
    json["from"] = position;
    json["to"] = position + 1;
    json["tracks"] = nlohmann::json::array();
    json["tracks"].push_back(nlohmann::json::object());
    json["tracks"][0]["id"] = track_id;
    std::cout << json.dump() << std::endl;

    cpr::Parameters parameters{{"diff", json.dump()}, {"revision", std::to_string(playlist->revision)}};
    cpr::Response response = cpr::Post(Url, parameters, get_base_headers(user_id_, token_));
    if (response.status_code == 200) {
      nlohmann::json http_response = nlohmann::json::parse(response.text);
      invocation_info request_info = get_invocation_info(http_response);
      playlist = setup_playlist(http_response["result"]);
      return true;
    }

    return false;
  }

} // namespace ymsummorizer::ymapi
