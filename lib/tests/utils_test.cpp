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

TEST( SentenceBoundaryTest, EmptyString ) {
    std::string testString;
    auto iter =
        Utils::sentenceBoundary( std::begin( testString ), std::end( testString ) );
    ASSERT_EQ( iter, std::end( testString ) );
}

TEST( SentenceBoundaryTest, SimpleSentence ) {
    std::string testString{"This is a simple sentence."};
    auto iter =
        Utils::sentenceBoundary( std::begin( testString ), std::end( testString ) );
    ASSERT_EQ( iter, --std::end( testString ) );
}

TEST( SentenceBoundaryTest, UnknownSentenceEnding ) {
    std::string testString{"Notice:"};
    auto iter =
        Utils::sentenceBoundary( std::begin( testString ), std::end( testString ) );
    ASSERT_EQ( iter, std::end( testString ) );
}

TEST( SentenceBoundaryTest, IterRangeSentence ) {
    std::string testString{
        "People of Earth, your attention, please. This is Prostetnic Vogon Jeltz of the "
        "Galactic Hyperspace Planning Council. As you will no doubt be aware, the plans "
        "for development of the outlying regions of the Galaxy require the building of a "
        "hyperspatial express route through your star system. And regrettably, your "
        "planet is one of those scheduled for demolition. The process will take slightly "
        "less than two of your Earth minutes. Thank you."};

    for ( auto start = std::begin( testString ); start != std::end( testString ); ) {
        auto last = Utils::sentenceBoundary( start, std::end( testString ) );
        std::cerr << testString.substr( std::distance( std::begin( testString ), start ),
                                        std::distance( start, last ) ) << std::endl;
        start = std::next( last );
    }
}

