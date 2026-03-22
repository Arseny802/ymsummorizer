#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

#include "hare/config.h"
#include "hare/config_default.h"
#include "hare/hare_loggers.h"
#include "hare/hlevels.h"
#include "mocs/manager_base_mock.h"
#include "storage/db_manager.h"
#include "storage/storage.hpp"

#include "../src/pch.h"

namespace ymsummorizer::storage {

class db_manager_test : public ::testing::Test {
 protected:
  void SetUp() override {
    log()->set_level(hare::hlevels::off);

    mock_manager_ = std::make_shared<db::manager_base_mock>();
    ON_CALL(*mock_manager_, connect(::testing::_))
      .WillByDefault(::testing::Return(true));
    ON_CALL(*mock_manager_, create_db())
      .WillByDefault(::testing::Return(true));
    ON_CALL(*mock_manager_, get_stored_settings())
      .WillByDefault(::testing::Return(std::optional<std::vector<common::setting>>{}));

    db_manager_ = std::make_unique<db_manager>(storage_types::sqlite3, "test_db");
    // The mock is injected via the friend class access to private members
    db_manager_->manager_pimpl = mock_manager_;
  }

  decltype((db_manager::connected_)) get_connected() { return db_manager_->connected_; }
  decltype((db_manager::settings_)) get_settings() { return db_manager_->settings_; }

  std::shared_ptr<db::manager_base_mock> mock_manager_;
  std::unique_ptr<db_manager> db_manager_;
};

TEST_F(db_manager_test, ConnectSuccess) {
  EXPECT_CALL(*mock_manager_, connect("test_db")).WillOnce(::testing::Return(true));
  EXPECT_CALL(*mock_manager_, create_db()).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->connect());
}

TEST_F(db_manager_test, ConnectFailureOnConnect) {
  EXPECT_CALL(*mock_manager_, connect("test_db")).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->connect());
}

TEST_F(db_manager_test, ConnectFailureOnCreateDb) {
  EXPECT_CALL(*mock_manager_, connect("test_db")).WillOnce(::testing::Return(true));
  EXPECT_CALL(*mock_manager_, create_db()).WillOnce(::testing::Return(false));

  EXPECT_FALSE(db_manager_->connect());
}

TEST_F(db_manager_test, Flash) {
  EXPECT_CALL(*mock_manager_, flash()).WillOnce(::testing::Return(true));

  get_connected() = true;

  EXPECT_TRUE(db_manager_->flash());
}

TEST_F(db_manager_test, GetStoredSettingFoundInCache) {
  get_settings()["test_key"] = "cached_value";

  common::setting setting;
  setting.key = "test_key";

  EXPECT_EQ(db_manager_->get_stored_setting(setting, "default"), "cached_value");
  // Should not call the mock
}

TEST_F(db_manager_test, GetStoredSettingFoundInDb) {
  common::setting setting;
  setting.key = "test_key";
  setting.value = "db_value";

  EXPECT_CALL(*mock_manager_, get_stored_setting(::testing::_))
      .WillOnce(::testing::DoAll(::testing::SetArgReferee<0>(setting), ::testing::Return(true)));

  EXPECT_EQ(db_manager_->get_stored_setting(setting, "default"), "db_value");
}

TEST_F(db_manager_test, GetStoredSettingNotFound) {
  common::setting setting;
  setting.key = "test_key";

  EXPECT_CALL(*mock_manager_, get_stored_setting(::testing::_)).WillOnce(::testing::Return(false));

  EXPECT_EQ(db_manager_->get_stored_setting(setting, "default"), "default");
}

TEST_F(db_manager_test, SetStoredSetting) {
  common::setting setting;
  setting.key = "test_key";
  setting.value = "test_value";

  EXPECT_CALL(*mock_manager_, set_stored_setting(setting)).WillOnce(::testing::Return(true));

  EXPECT_TRUE(db_manager_->set_stored_setting(setting));
}

// Add more tests as needed for other methods

}  // namespace ymsummorizer::storage
