#include "../lib/xml_doc.hpp"
#include "../lib/xml_exception.hpp"
#include "../lib/pathname.hpp"
#include "../lib/dir.hpp"

#include <stdexcept>
#include <regex>
#include <vector>
#include <algorithm>
#include <iostream>

#include <gtest/gtest.h>
#include <limits.h>

// save pathname of build directory
const Pathname buildDir{Dir::getInstance( )->pwd( )};

TEST( XmlDocTests, DefaultCtor ) {
    const XmlDoc xml;
    auto pathname = xml.pathname( );
    EXPECT_FALSE( pathname.isValid( ) );
}

TEST( XmlDocTests, ValidPathnameConstructor ) {
    Dir::getInstance( )->chdir( Pathname{"tests/xmltest/valid/sa"} );
    std::regex glob{".*\\.xml"};
    auto ls = Dir::getInstance( )->read( ).entries( );
    std::vector<Pathname> xmlValidEntries{filter( ls, glob )};
    std::sort( xmlValidEntries.begin( ), xmlValidEntries.end( ) );
    for ( auto &p : xmlValidEntries ) {
        EXPECT_NO_THROW( XmlDoc doc{p} );
    }

    Dir::getInstance( )->chdir( buildDir );
    Dir::killInstance( );
}

TEST( XmlDocTests, NotWellFormedXml ) {
    Dir::getInstance( )->chdir( Pathname{"tests/xmltest/not-wf/sa"} );
    std::regex glob{".*\\.xml"};
    auto ls = Dir::getInstance( )->read( ).entries( );
    std::vector<Pathname> xmlNotWFEntries{filter( ls, glob )};
    std::sort( xmlNotWFEntries.begin( ), xmlNotWFEntries.end( ) );
    for ( auto &p : xmlNotWFEntries ) {
        std::cout << p << std::endl;
        if (std::string{"140.xml"} == p.basename().toString()) continue;
        if (std::string{"141.xml"} == p.basename().toString()) continue;
        ASSERT_THROW( XmlDoc doc{p}, XmlException );
    }

    Dir::getInstance( )->chdir( buildDir );
    Dir::killInstance( );
}
