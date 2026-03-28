#pragma once

#include "../db/manager_base.h"

namespace ymsummorizer::storage::data {

class bot_info_generator {
 public:
  explicit bot_info_generator(db::manager_base_ptr db);
  ~bot_info_generator();

  bool generate_bot_info();

 private:
  db::manager_base_ptr db_;
};

}  // namespace ymsummorizer::storage::data
