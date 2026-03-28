#pragma once

#include "../db/manager_base.h"

namespace ymsummorizer::storage::data {

class user_generator {
 public:
  explicit user_generator(db::manager_base_ptr db);
  ~user_generator();

  bool generate_users();

 private:
  db::manager_base_ptr db_;
};

}  // namespace ymsummorizer::storage::data
