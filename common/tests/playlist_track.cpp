#include <gtest/gtest.h>
#include "common/playlist_track.h"

using namespace ymsummorizer::common;

TEST(PlaylistTrackTest, FormatReturnsCorrectString) {
    playlist_track pt;
    pt.id = "pt1";
    pt.name = "My Track";
    pt.group_id = "g1";

    std::string expected = "pt1 My Track g1";
    EXPECT_EQ(pt.format(), expected);
}

TEST(PlaylistTrackTest, IsValidNameReturnsFalseForEmpty) {
    EXPECT_FALSE(playlist_track::is_valid_name(""));
}

TEST(PlaylistTrackTest, IsValidNameReturnsFalseForTooLong) {
    std::string long_name(65, 'x');
    EXPECT_FALSE(playlist_track::is_valid_name(long_name));
}

TEST(PlaylistTrackTest, IsValidNameReturnsTrueForValid) {
    EXPECT_TRUE(playlist_track::is_valid_name("Valid Track Name"));
}
