#include "../lib/xml_doc.hpp"
#include "../lib/xml_exception.hpp"
#include "../lib/pathname.hpp"
#include "../lib/dir.hpp"

#include <stdexcept>
#include <regex>
#include <vector>
#include <algorithm>
#include <iostream>

#include "gtest/gtest.h"
#include <limits.h>
#include <glog/logging.h>

static const Pathname buildDir_{Dir::getInstance( )->pwd( )};

class XmlDocTests : public ::testing::Test {
protected:
    static void SetUpTestCase() {
    }

    virtual auto SetUp( ) -> void {
        std::cerr << __PRETTY_FUNCTION__ << std::endl;
        Dir::getInstance( )->chdir( buildDir_ );
    }

    virtual auto TearDown( ) -> void {
        std::cerr << __PRETTY_FUNCTION__ << std::endl;
        Dir::getInstance( )->chdir( buildDir_ );
        Dir::killInstance( );
    }
};

TEST_F( XmlDocTests, DefaultCtor ) {
    LOG(INFO) << "blah blah";
    const XmlDoc xml;
    auto pathname = xml.pathname( );
    EXPECT_FALSE( pathname.isValid( ) );
}

TEST_F( XmlDocTests, ValidPathnameConstructor ) {
    Dir::getInstance( )->chdir( Pathname{"tests/xmltest/valid/sa"} );
    std::regex glob{".*\\.xml"};
    auto ls = Dir::getInstance( )->read( ).entries( );
    std::vector<Pathname> xmlValidEntries{filter( ls, glob )};
    std::sort( xmlValidEntries.begin( ), xmlValidEntries.end( ) );
    for ( auto &p : xmlValidEntries ) {
        EXPECT_NO_THROW( XmlDoc doc{p} );
    }
}

TEST_F( XmlDocTests, NotWellFormedXml ) {
    std::cerr << Dir::getInstance( )->pwd( ).toString( ) << std::endl;
    Dir::getInstance( )->chdir( Pathname{"tests/xmltest/not-wf/sa"} );
    std::regex glob{".*\\.xml"};
    auto ls = Dir::getInstance( )->read( ).entries( );
    std::vector<Pathname> xmlNotWFEntries{filter( ls, glob )};
    std::sort( xmlNotWFEntries.begin( ), xmlNotWFEntries.end( ) );
    for ( auto &p : xmlNotWFEntries ) {
        if ( std::string{"140.xml"} == p.basename( ).toString( ) )
            continue;
        if ( std::string{"141.xml"} == p.basename( ).toString( ) )
            continue;
        ASSERT_THROW( XmlDoc doc{p}, XmlException );
    }
}
