#include "lib/utils.hpp"
#include <iostream>
#include <vector>
#include <string>

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

TEST( ParagraphBoundaryTest, NoParagraph ) {
    std::string testString{
        "Trin Tragula — for that was his name — was a dreamer, a thinker, a speculative "
        "philosopher or, as his wife would have it, an idiot."};
    auto paragraphEnd =
        Utils::paragraphBoundary( std::begin( testString ), std::end( testString ) );
    ASSERT_TRUE(
        testString ==
        testString.substr( 0, std::distance( std::begin( testString ), paragraphEnd ) ) );
}

TEST( ParagraphBoundaryTest, SimpleParagraph ) {
    std::string testString{
        "Trin Tragula — for that was his name — was a dreamer, a thinker, a speculative "
        "philosopher or, as his wife would have it, an idiot.\nAnd she would nag him "
        "incessantly about the utterly inordinate amount of time he spent staring out "
        "into space, or mulling over the mechanics of safety pins, or doing "
        "spectrographic analyses of pieces of fairy cake."};
    auto paragraphEnd =
        Utils::paragraphBoundary( std::begin( testString ), std::end( testString ) );
    std::cerr << testString.substr( 0, std::distance( std::begin( testString ),
                                                      paragraphEnd ) ) << std::endl;
}

TEST( ParagraphBoundaryTest, TrailingSpace ) {
    std::string testString{
        "Bis 2008 war er ein Jahr lang persönlicher Mitarbeiter des Aargauer "
        "Regierungsrates\n        Urs Hofmann.\n        Von dem gemässigten SP-Mann hat "
        "er viel gelernt, nicht zuletzt hat er dort gesehen, wie das Regieren\n        "
        "funktioniert. 2010 versteuerte er 45 000 Franken Einkommen. Seit Dezember 2011 "
        "verdient er sein Salär im\n        Nationalrat, durchschnittlich 9554 Franken "
        "pro Monat. Steuerbares Vermögen: null.\n    "};
    std::vector<std::string> sentences;
    for ( auto start = std::begin( testString ); start != std::end( testString ); ) {
        auto paragraphEnd = Utils::paragraphBoundary( start, std::end( testString ) );
        auto paragraph =
            testString.substr( std::distance( std::begin( testString ), start ),
                               std::distance( start, paragraphEnd ) );
        Utils::ltrim( paragraph );
        sentences.push_back( paragraph );
        start = paragraphEnd;
    }
    for ( auto sentence : sentences ) {
        sentence.erase(
            Utils::collapseWhiteSpace( std::begin( sentence ), std::end( sentence ) ),
            std::end( sentence ) );
        if ( !sentence.empty( ) ) {
            std::cerr << "'" << sentence << "'" << std::endl;
        }
    }
}

