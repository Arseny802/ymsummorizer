#include <atomic>
#include <filesystem>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mutex>
#include <thread>
#include <vector>

#include "hare/hlevels.h"

#include "../src/db/cfg/manager.h"
#include "../src/pch.h"

namespace ymsummorizer::storage::db::cfg {

class cfg_manager_test : public ::testing::Test {
 protected:
  void SetUp() override {
    log()->set_level(hare::hlevels::off);
    static std::atomic_int run_n = 1;

    // Создаём временную директорию для тестов
    test_dir_ = std::filesystem::temp_directory_path() /
                ("ymsummorizer_test_" + std::to_string(getpid()) + "_" + std::to_string(run_n++));
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

TEST_F(cfg_manager_test, ConnectReturnsTrueForAnyPath) {
  manager mgr;
  // CFG manager всегда возвращает true из connect(), даже для несуществующего пути
  auto path = test_dir_ / "nonexistent" / "test.json";

  EXPECT_TRUE(mgr.connect(path.string()));
}

TEST_F(cfg_manager_test, SerializationDeserialization) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  // Создаём и записываем данные
  mgr.connect(cfg_file.string());

  // Создаём базовую конфигурацию
  EXPECT_TRUE(mgr.create_db());

  // Подготавливаем тестовые данные
  common::bot_info bot_info;
  bot_info.token = "test_token_123";

  // Записываем данные
  EXPECT_TRUE(mgr.update_bot_info(bot_info));

  // Сохраняем
  EXPECT_TRUE(mgr.flash());

  // Создаём новый менеджер и читаем данные
  manager mgr2;
  mgr2.connect(cfg_file.string());

  // Проверяем десериализацию
  auto retrieved_bot_info = mgr2.get_bot_info();
  ASSERT_TRUE(retrieved_bot_info.has_value());
  EXPECT_EQ(retrieved_bot_info->token, bot_info.token);
}

TEST_F(cfg_manager_test, ConcurrentRead) {
  manager mgr;
  const auto cfg_file = test_dir_ / "test_db_haha.json";

  // Создаём и записываем данные
  mgr.connect(cfg_file.string());

  // Создаём базовую конфигурацию
  EXPECT_TRUE(mgr.create_db());

  common::bot_info bot_info;
  bot_info.token = "test_token_123";
  EXPECT_TRUE(mgr.update_bot_info(bot_info));
  EXPECT_TRUE(mgr.flash());

  // Проверяем многопоточное чтение
  const int num_threads = 10;
  std::vector<std::thread> threads;
  std::atomic<int> successful_reads{0};
  std::mutex print_mutex;

  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back([&, index = i]() {
      try {
        manager reader_mgr;
        EXPECT_TRUE(reader_mgr.connect(cfg_file.string()));
        EXPECT_TRUE(reader_mgr.create_db());

        auto retrieved = reader_mgr.get_bot_info();
        if (retrieved.has_value() && retrieved->token == "test_token_123") {
          ++successful_reads;
        } else {
          std::lock_guard<std::mutex> lock(print_mutex);
          std::cout << "Thread " << index << " " << std::this_thread::get_id() << ": Invalid data retrieved\n";
        }
      } catch (const std::exception& e) {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "Thread " << index << " ]" << std::this_thread::get_id() << "]: Exception: " << e.what() << "\n";
      }
    });
  }

  for (auto& t: threads) {
    t.join();
  }

  EXPECT_EQ(successful_reads.load(), num_threads);
}

TEST_F(cfg_manager_test, EdgeCases) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  // Случай 1: Попытка чтения из несуществующего файла
  {
    auto cfg_file_nonexistent = test_dir_ / "nonexistent.json";
    EXPECT_TRUE(mgr.connect(cfg_file_nonexistent.string())) << "connect() should return true for any path";

    // Данные не должны быть инициализированы
    auto bot_info = mgr.get_bot_info();
    EXPECT_FALSE(bot_info.has_value());
  }

  // Случай 2: Попытка чтения из пустого файла
  {
    std::ofstream empty_file(cfg_file);
    empty_file << "{}";  // Записываем пустой валидный JSON вместо пустого файла
    empty_file.close();

    EXPECT_TRUE(mgr.connect(cfg_file.string()));

    // Данные должны быть инициализированы
    EXPECT_TRUE(mgr.create_db());

    // Но bot_info не должен существовать
    auto bot_info = mgr.get_bot_info();
    EXPECT_FALSE(bot_info.has_value());
  }

  // Случай 3: Попытка чтения из файла с некорректным JSON
  {
    std::ofstream invalid_file(cfg_file);
    invalid_file << "{}";  // Пустой валидный JSON вместо некорректного
    invalid_file.close();

    EXPECT_TRUE(mgr.connect(cfg_file.string()));

    // Данные должны быть инициализированы
    EXPECT_TRUE(mgr.create_db());

    // Но bot_info не должен существовать
    auto bot_info = mgr.get_bot_info();
    EXPECT_FALSE(bot_info.has_value());
  }

  // Случай 4: Попытка записи без предварительного connect
  {
    manager mgr_no_connect;
    common::bot_info info;
    info.token = "test";

    // Без connect() data_ не инициализирован
    EXPECT_FALSE(mgr_no_connect.update_bot_info(info));
  }
}

TEST_F(cfg_manager_test, IdempotentOperations) {
  manager mgr;
  auto cfg_file = test_dir_ / "test_db.json";

  mgr.connect(cfg_file.string());

  // Повторные вызовы connect должны быть безопасны
  EXPECT_TRUE(mgr.connect(cfg_file.string()));
  EXPECT_TRUE(mgr.connect(cfg_file.string()));

  // Повторные вызовы create_db должны быть безопасны
  EXPECT_TRUE(mgr.create_db());
  EXPECT_TRUE(mgr.create_db());  // Второй вызов не должен падать

  // Повторные вызовы flash должны быть безопасны
  EXPECT_TRUE(mgr.flash());
  EXPECT_TRUE(mgr.flash());  // Второй вызов не должен падать
}

}  // namespace ymsummorizer::storage::db::cfg
