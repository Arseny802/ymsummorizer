#pragma once
#include "common/common.hpp"

#include <memory>
#include <vector>

namespace ymsummorizer::tgbot {
  struct ymsummorizer_callback_result {
    using ptr = std::shared_ptr<ymsummorizer_callback_result>;
    virtual ~ymsummorizer_callback_result() = default;

    bool ok = false;
    std::string error_msg{};
  };

  struct group_list_callback_result: public ymsummorizer_callback_result {
    using ptr = std::shared_ptr<group_list_callback_result>;

    std::vector<common::group> groups{};
    //std::vector<common::user> filtered_users{};
    std::map<std::string, common::user> filtered_users{};

    group_list_callback_result() = default;
    group_list_callback_result(const group_list_callback_result&) = delete;
    group_list_callback_result& operator=(const group_list_callback_result&) = delete;
    group_list_callback_result(group_list_callback_result&&) = default;
    group_list_callback_result& operator=(group_list_callback_result&&) = default;
  };

} // namespace ymsummorizer::tgbot
