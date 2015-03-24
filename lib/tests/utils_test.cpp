#include "lib/utils.hpp"
#include <iostream>

#include "gtest/gtest.h"
#include <limits.h>

TEST( CollapseWhiteSpaceTest, EmptyString ) {
    std::string testString;
    auto iter =
        Utils::collapseWhiteSpace( std::begin( testString ), std::end( testString ) );
    ASSERT_EQ( iter, std::end( testString ) );
}

TEST( CollapseWhiteSpaceTest, WhiteSpaceString ) {
    std::string testString{" \t\n  "};
    auto iter =
        Utils::collapseWhiteSpace( std::begin( testString ), std::end( testString ) );
    testString.erase( iter, std::end( testString ) );
    ASSERT_STREQ( "", testString.c_str( ) );
}

TEST( CollapseWhiteSpaceTest, WhiteSpacesInString ) {
    std::string testString{"this    is   a  string"};
    auto iter =
        Utils::collapseWhiteSpace( std::begin( testString ), std::end( testString ) );
    testString.erase( iter, std::end( testString ) );
    ASSERT_STREQ( "this is a string", testString.c_str( ) );
}

