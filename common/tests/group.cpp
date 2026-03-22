#include <gtest/gtest.h>
#include "common/group.h"

using namespace ymsummorizer::common;

TEST(GroupTest, FormatReturnsCorrectString) {
    group g;
    g.id = "g1";
    g.name = "My Group";
    g.user_ids = {"u1", "u2", "u3"};

    std::string expected = "My Group ('g1') with 3 users.";
    EXPECT_EQ(g.format(), expected);
}

TEST(GroupTest, IsValidNameReturnsFalseForEmpty) {
    EXPECT_FALSE(group::is_valid_name(""));
}

TEST(GroupTest, IsValidNameReturnsFalseForTooLong) {
    std::string long_name(65, 'x');
    EXPECT_FALSE(group::is_valid_name(long_name));
}

TEST(GroupTest, IsValidNameReturnsTrueForValid) {
    EXPECT_TRUE(group::is_valid_name("Valid Group Name"));
}
