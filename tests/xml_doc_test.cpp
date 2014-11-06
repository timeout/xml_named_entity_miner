#include "../lib/xml_doc.hpp"
#include "../lib/pathname.hpp"
#include "../lib/dir.hpp"

#include <stdexcept>
#include <regex>
#include <vector>
#include <algorithm>
#include <iostream>

#include <gtest/gtest.h>
#include <limits.h>

const Pathname buildDir{Dir::getInstance( )->pwd( )};

TEST( XmlDocTests, DefaultCtor ) {
    const XmlDoc xml;
    auto pathname = xml.pathname( );
    EXPECT_FALSE( pathname.isValid( ) );
}

TEST( XmlDocTests, PathnameConstructor ) {
    Dir::getInstance( )->chdir( Pathname{"tests"} );
    // std::regex glob{".*"};
    std::regex glob{".*\\.xml"};
    auto ls = Dir::getInstance( )->read( ).filter( glob );
    for ( auto &p : ls ) {
        std::cout << p << std::endl;
        std::string bn = p.basename( ).toString( );
        if ( p.basename( ).toString( ) == "valid.xml" ) {
            std::cout << p.basename( ) << " exists!" << std::endl;
        }
        if ( p.basename( ).toString( ) == "valid.xml" ) {
            std::cout << p.basename( ) << " exists!" << std::endl;
        }
    }

    Dir::getInstance( )->chdir( buildDir );
    Dir::killInstance( );
}

