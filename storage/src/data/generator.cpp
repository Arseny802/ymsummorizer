#include "generator.h"
#include "hare/defs.h"

#include "administrators.h"
#include "bot_info.h"
#include "groups.h"
#include "users.h"
#include <memory>

namespace ymsummorizer::storage::data {

generator::generator(db::manager_base_ptr db)
    : user_gen_(std::make_unique<user_generator>(db)),
      group_gen_(std::make_unique<group_generator>(db)),
      admin_gen_(std::make_unique<administrator_generator>(db)),
      bot_info_gen_(std::make_unique<bot_info_generator>(db)) {
  AUTOTRACE;
}

generator::~generator() {
  AUTOTRACE;
}

bool generator::generate_all() {
  AUTOMEASURE;
  // Генерация всех данных должна происходить в правильном порядке
  // зависимости: пользователи -> группы -> администраторы, информация о боте

  if (!user_gen_->generate_users()) {
    log()->error("Failed to generate users.");
    return false;
  }

  if (!group_gen_->generate_groups()) {
    log()->error("Failed to generate groups.");
    return false;
  }

  if (!admin_gen_->generate_administrators()) {
    log()->error("Failed to generate administrators.");
    return false;
  }

  if (!bot_info_gen_->generate_bot_info()) {
    log()->error("Failed to generate bot info.");
    return false;
  }

  return true;
}

}  // namespace ymsummorizer::storage::data
