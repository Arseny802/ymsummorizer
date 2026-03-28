#pragma once

#include "../db/manager_base.h"

namespace ymsummorizer::storage::data {

class administrator_generator {
 public:
  explicit administrator_generator(db::manager_base_ptr db);
  ~administrator_generator();

  bool generate_administrators();

 private:
  db::manager_base_ptr db_;
};

}  // namespace ymsummorizer::storage::data
