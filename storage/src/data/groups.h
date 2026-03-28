#pragma once

#include "../db/manager_base.h"

namespace ymsummorizer::storage::data {

class group_generator {
 public:
  explicit group_generator(db::manager_base_ptr db);
  ~group_generator();

  bool generate_groups();

 private:
  db::manager_base_ptr db_;
};

}  // namespace ymsummorizer::storage::data
