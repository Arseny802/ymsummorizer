#include <gtest/gtest.h>
#include "common/user.h"

using namespace ymsummorizer::common;

TEST(UserTest, FormatReturnsCorrectString) {
    user u;
    u.id = "123";
    u.name = "John Doe";
    u.login_tg = "@johndoe";
    u.login_yandex = "johndoe@yandex.ru";
    u.token = "abc123";

    std::string expected = "123 John Doe @johndoe johndoe@yandex.ru";
    EXPECT_EQ(u.format(), expected);
}

TEST(UserTest, HideSpecialCharsHandlesUnderscore) {
    std::string input = "hello_world";
    std::string expected = "hello\\_world";
    EXPECT_EQ(user::hide_spetial_chars(input), expected);
}

TEST(UserTest, HideSpecialCharsHandlesMultipleUnderscores) {
    std::string input = "user_name_test";
    std::string expected = "user\\_name\\_test";
    EXPECT_EQ(user::hide_spetial_chars(input), expected);
}

TEST(UserTest, HideSpecialCharsNoSpecialChars) {
    std::string input = "normalstring";
    std::string expected = "normalstring";
    EXPECT_EQ(user::hide_spetial_chars(input), expected);
}
