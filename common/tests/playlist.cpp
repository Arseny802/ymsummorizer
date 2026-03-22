#include <gtest/gtest.h>
#include "common/playlist.h"

using namespace ymsummorizer::common;

TEST(PlaylistTest, FormatReturnsCorrectString) {
    playlist p;
    p.id = "p1";
    p.name = "My Playlist";
    p.group_id = "g1";

    std::string expected = "p1 My Playlist g1";
    EXPECT_EQ(p.format(), expected);
}

TEST(PlaylistTest, IsValidNameReturnsFalseForEmpty) {
    EXPECT_FALSE(playlist::is_valid_name(""));
}

TEST(PlaylistTest, IsValidNameReturnsFalseForTooLong) {
    std::string long_name(65, 'x');
    EXPECT_FALSE(playlist::is_valid_name(long_name));
}

TEST(PlaylistTest, IsValidNameReturnsTrueForValid) {
    EXPECT_TRUE(playlist::is_valid_name("Valid Playlist Name"));
}
