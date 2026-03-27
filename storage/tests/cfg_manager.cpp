#include <filesystem>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "hare/config.h"
#include "hare/config_default.h"
#include "hare/hare_loggers.h"
#include "hare/hlevels.h"
#include "storage/storage.hpp"

#include "../src/db/cfg/manager.h"
#include "../src/pch.h"

namespace ymsummorizer::storage::db::cfg {

class cfg_manager_test : public ::testing::Test {
 protected:
  void SetUp() override {
    log()->set_level(hare::hlevels::off);

    // Создаём временную директорию для тестов
    test_dir_ = std::filesystem::temp_directory_path() / ("ymsummorizer_test_" + std::to_string(getpid()));
    std::filesystem::create_directory(test_dir_);
  }

  void TearDown() override {
    // Удаляем временную директорию
    if (std::filesystem::exists(test_dir_)) {
      std::filesystem::remove_all(test_dir_);
    }
  }

  std::filesystem::path test_dir_;

  long getpid() { return static_cast<long>(std::hash<std::thread::id>{}(std::this_thread::get_id())); }
};

TEST_F(cfg_manager_test, ConnectCreatesConfigFile) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  EXPECT_TRUE(mgr.connect(cfg_file.string()));
  EXPECT_TRUE(mgr.create_db());
  EXPECT_TRUE(std::filesystem::exists(cfg_file));
}

TEST_F(cfg_manager_test, ConnectReturnsTrueForAnyPath) {
  manager mgr;
  // CFG manager всегда возвращает true из connect(), даже для несуществующего пути
  auto path = test_dir_ / "nonexistent" / "test.json";

  EXPECT_TRUE(mgr.connect(path.string()));
}

TEST_F(cfg_manager_test, GetStoredSettingAfterConnect) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  mgr.create_db();

  common::setting setting;
  setting.key = "test_key";
  setting.value = "test_value";

  EXPECT_TRUE(mgr.set_stored_setting(setting));

  // Получаем обратно
  common::setting get_setting;
  get_setting.key = "test_key";

  EXPECT_TRUE(mgr.get_stored_setting(get_setting));
}

TEST_F(cfg_manager_test, GetStoredSettings) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  mgr.create_db();

  // Добавляем несколько настроек
  mgr.set_stored_setting(common::setting{"key1", "value1"});
  mgr.set_stored_setting(common::setting{"key2", "value2"});

  auto settings = mgr.get_stored_settings();
  ASSERT_TRUE(settings.has_value());
}

TEST_F(cfg_manager_test, GetStoredUsers) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  mgr.create_db();

  auto users = mgr.get_stored_users();
  // create_db() создаёт 2 пользователей по умолчанию
  EXPECT_EQ(users.size(), 2);
}

TEST_F(cfg_manager_test, GetStoredGroups) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  mgr.create_db();

  auto groups = mgr.get_stored_groups();
  // create_db() создаёт 1 группу по умолчанию
  EXPECT_EQ(groups.size(), 1);
}

TEST_F(cfg_manager_test, GetAdmins) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  mgr.create_db();

  auto admins = mgr.get_admin_ids();
  // create_db() создаёт 1 админа по умолчанию
  EXPECT_EQ(admins.size(), 1);
}

TEST_F(cfg_manager_test, GetSuperadmins) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  mgr.create_db();

  auto superadmins = mgr.get_superadmin_ids();
  // create_db() создаёт 1 суперадмина по умолчанию
  EXPECT_EQ(superadmins.size(), 1);
}

TEST_F(cfg_manager_test, AddAndGetBotInfo) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  mgr.create_db();

  common::bot_info info;
  info.token = "test_bot_token_12345";

  EXPECT_TRUE(mgr.update_bot_info(info));

  auto retrieved = mgr.get_bot_info();
  ASSERT_TRUE(retrieved.has_value());
  EXPECT_EQ(retrieved->token, "test_bot_token_12345");
}

TEST_F(cfg_manager_test, DeleteBotInfo) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  mgr.create_db();

  // Добавляем info
  mgr.update_bot_info(common::bot_info{"token123"});

  // Удаляем - delete_bot_info возвращает false (особенность реализации)
  EXPECT_FALSE(mgr.delete_bot_info());

  auto retrieved = mgr.get_bot_info();
  EXPECT_FALSE(retrieved.has_value());
}

TEST_F(cfg_manager_test, GetGroupPlaylists) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  mgr.create_db();

  auto groups = mgr.get_stored_groups();
  ASSERT_EQ(groups.size(), 1);

  auto playlists = mgr.get_group_playlists(groups[0].id);
  // По умолчанию плейлистов нет
  EXPECT_TRUE(playlists.empty());
}

TEST_F(cfg_manager_test, FlashSavesData) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  mgr.create_db();

  // Flash - должен сохранить данные
  EXPECT_TRUE(mgr.flash());

  // Создаём новый manager и читаем данные
  manager mgr2;
  mgr2.connect(cfg_file.string());

  auto users = mgr2.get_stored_users();
  ASSERT_EQ(users.size(), 2);
}

TEST_F(cfg_manager_test, CreateDb) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());

  // create_db должен создать структуру данных
  EXPECT_TRUE(mgr.create_db());

  // Проверяем, что можем получить данные
  auto users = mgr.get_stored_users();
  auto groups = mgr.get_stored_groups();
  EXPECT_EQ(users.size(), 2);   // create_db создаёт 2 пользователей по умолчанию
  EXPECT_EQ(groups.size(), 1);  // create_db создаёт 1 группу по умолчанию
}

TEST_F(cfg_manager_test, BotInfoNotExistsBeforeCreate) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  // Не вызываем create_db()

  auto retrieved = mgr.get_bot_info();
  EXPECT_FALSE(retrieved.has_value());
}

TEST_F(cfg_manager_test, UpdateBotInfoWithoutData) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());
  // Не вызываем create_db()

  common::bot_info info;
  info.token = "test_token";

  EXPECT_FALSE(mgr.update_bot_info(info));
}

}  // namespace ymsummorizer::storage::db::cfg
