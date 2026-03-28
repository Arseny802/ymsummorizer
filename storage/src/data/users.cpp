#include "users.h"
#include "hare/defs.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>

namespace ymsummorizer::storage::data {

user_generator::user_generator(db::manager_base_ptr db): db_(std::move(db)) {
  AUTOTRACE;
}

user_generator::~user_generator() {
  AUTOTRACE;
}

bool user_generator::generate_users() {
  try {
    // Генерация двух примеров пользователей
    common::user user1;
    user1.id = boost::uuids::to_string(boost::uuids::random_generator()());
    user1.name = "User01_name";
    user1.login_tg = "User01_login_tg";
    user1.login_yandex = "User01_login_yandex";
    user1.token = "User01_yandex_token";

    common::user user2;
    user2.id = boost::uuids::to_string(boost::uuids::random_generator()());
    user2.name = "User02_name";
    user2.login_tg = "User02_login_tg";
    user2.login_yandex = "User02_login_yandex";
    user2.token = "User02_yandex_token";

    // Сохранение пользователей через базовый интерфейс хранилища
    if (!db_->add_user(user1) || !db_->add_user(user2)) {
      log()->error("Failed to add generated users to database.");
      return false;
    }

    return true;
  } catch (const std::exception& e) {
    log()->error("Error during user generation: {0}", e.what());
    return false;
  }
}

}  // namespace ymsummorizer::storage::data
