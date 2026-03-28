#pragma once

#include "../db/manager_base.h"
#include <memory>

namespace ymsummorizer::storage::data {

class generator {
 public:
  explicit generator(db::manager_base_ptr db);
  ~generator();
  bool generate_all();

 private:
  std::unique_ptr<class user_generator> user_gen_;
  std::unique_ptr<class group_generator> group_gen_;
  std::unique_ptr<class administrator_generator> admin_gen_;
  std::unique_ptr<class bot_info_generator> bot_info_gen_;
};

}  // namespace ymsummorizer::storage::data
