#include "xml_schema_parser.hpp"
#include "dir.hpp"

#include <fstream>

#include "gtest/gtest.h"
#include <limits.h>

static const Pathname buildDir_{Dir::getInstance( )->pwd( )};

class XmlSchemaParserTests : public testing::Test {
protected:
    virtual auto SetUp( ) -> void { Dir::getInstance( )->chdir( buildDir_ ); }
};

TEST_F( XmlSchemaParserTests, DefaultCtor ) {
    XmlSchemaParser schemaParser;
    EXPECT_FALSE( schemaParser.errorHandler( ).hasErrors( ) );
    EXPECT_TRUE( schemaParser.get( ) == nullptr );
}

TEST_F( XmlSchemaParserTests, CtorValidSchemas ) {
    Dir::getInstance( )->chdir( Pathname{"tests/schemas/valid"} );
    std::regex glob{".*\\.xsd"};
    auto ls = Dir::getInstance( )->read( ).entries( );
    std::vector<Pathname> schemaValidEntries{filter( ls, glob )};
    for ( Pathname &p : schemaValidEntries ) {
        std::ifstream f{p.toString( ), std::ios::in};
        ASSERT_TRUE( f.is_open( ) );
        const XmlDoc validSchema{f};
        XmlSchemaParser validParser;
        validSchema >> validParser;
        EXPECT_FALSE( validParser.errorHandler( ).hasErrors( ) );
        EXPECT_TRUE( validParser.get( ) != nullptr );
    }
}

// TEST_F( XmlSchemaParserTests, CtorInvalidSchemas ) {
//     std::ifstream g{"", std::ios::in};
//     ASSERT_TRUE( g.is_open( ) );
//     const XmlDoc invalidSchema{g};
//     XmlSchemaParser invalidParser;
//     invalidSchema >> invalidParser;
//     EXPECT_TRUE( invalidParser.errorHandler( ).hasErrors( ) );
//     EXPECT_FALSE( invalidParser.get( ) != nullptr );
// }
