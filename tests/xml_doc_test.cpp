#include "xml_doc.hpp"
#include "pathname.hpp"
#include "dir.hpp"

#include <stdexcept>
#include <regex>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include <limits.h>

static const Pathname buildDir_{Dir::getInstance( )->pwd( )};

class XmlDocTests : public ::testing::Test {
protected:
    virtual auto SetUp( ) -> void { Dir::getInstance( )->chdir( buildDir_ ); }
    auto testable( const XmlDoc &xml ) const -> bool { return xml ? true : false; }
};

TEST_F( XmlDocTests, DefaultCtor ) {
    const XmlDoc xml;
    EXPECT_FALSE( xml.errorHandler( ).hasErrors( ) );
    EXPECT_STREQ( "", xml.toString( ).c_str( ) );
    ASSERT_EQ( false, testable( xml ) );
}

TEST_F( XmlDocTests, ValidPathnameConstructor ) {
    Dir::getInstance( )->chdir( Pathname{"tests/xmltest/valid/sa"} );
    std::regex glob{".*\\.xml"};
    auto ls = Dir::getInstance( )->read( ).entries( );
    std::vector<Pathname> xmlValidEntries{filter( ls, glob )};
    std::sort( xmlValidEntries.begin( ), xmlValidEntries.end( ) );

    for ( auto &p : xmlValidEntries ) {
        if ( p.basename( ) == Pathname{"012.xml"} )
            continue;
        if ( p.basename( ) == Pathname{"045.xml"} )
            continue;
        if ( p.basename( ) == Pathname{"095.xml"} )
            continue;
        XmlDoc xml;
        std::ifstream f;
        try {
            f = std::ifstream{p.toString( ), std::ios::in};
        } catch ( ... ) {
            std::cerr << "ERROR: could not open '" << p << "'" << std::endl;
            throw;
        }
        f >> xml;
        EXPECT_FALSE( xml.errorHandler( ).hasErrors( ) );
        if ( xml.errorHandler( ).hasErrors( ) ) {
            std::cerr << p << std::endl;
            std::cerr << xml.errorHandler( ) << std::endl;
        }
        EXPECT_FALSE( !xml );
        ASSERT_EQ( true, testable( xml ) );
    }
}

TEST_F( XmlDocTests, NotWellFormedXml ) {
    Dir::getInstance( )->chdir( Pathname{"tests/xmltest/not-wf/sa"} );
    std::regex glob{".*\\.xml"};
    auto ls = Dir::getInstance( )->read( ).entries( );
    std::vector<Pathname> xmlNotWFEntries{filter( ls, glob )};
    std::sort( xmlNotWFEntries.begin( ), xmlNotWFEntries.end( ) );
    for ( auto &p : xmlNotWFEntries ) {
        if ( p.basename( ) == Pathname{"050.xml"} )
            continue;
        if ( p.basename( ) == Pathname{"140.xml"} )
            continue;
        if ( p.basename( ) == Pathname{"141.xml"} )
            continue;
        XmlDoc xml;
        std::ifstream f;
        try {
            f = std::ifstream{p.toString( ), std::ios::in};
        } catch ( ... ) {
            std::cerr << "ERROR: could not open '" << p << "'" << std::endl;
            throw;
        }
        f >> xml;
        EXPECT_TRUE( xml.errorHandler( ).hasErrors( ) );
        if ( !xml.errorHandler( ).hasErrors( ) ) {
            std::cerr << p << std::endl;
        }
    }
}
