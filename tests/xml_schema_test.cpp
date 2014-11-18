#include "xml_schema.hpp"
#include "xml_schema_parser.hpp"
#include "dir.hpp"

#include <fstream>

#include "gtest/gtest.h"
#include <limits.h>

static const Pathname buildDir_{Dir::getInstance( )->pwd( )};

class XmlSchemaTests : public testing::Test {
protected:
    virtual auto SetUp( ) -> void { Dir::getInstance( )->chdir( buildDir_ ); }
};

TEST_F( XmlSchemaTests, DefaultCtor ) {
    XmlSchema schema;
    EXPECT_FALSE( schema.errorHandler( ).hasErrors( ) );
    EXPECT_TRUE( schema.get( ) == nullptr );
}

TEST_F( XmlSchemaTests, CtorValidSchemas ) {
    Dir::getInstance( )->chdir( Pathname{"tests/schemas/valid"} );
    std::regex glob{".*\\.xsd"};
    auto ls = Dir::getInstance( )->read( ).entries( );
    std::vector<Pathname> schemaValidEntries{filter( ls, glob )};
    for ( Pathname &p : schemaValidEntries ) {
        std::ifstream f{p.toString( ), std::ios::in};
        ASSERT_TRUE( f.is_open( ) );
        const XmlDoc validSchema{f};
        XmlSchema schema;
        XmlSchemaParser validParser;
        validSchema >> validParser >> schema;
        EXPECT_FALSE( schema.errorHandler( ).hasErrors( ) );
        if ( schema.errorHandler( ).hasErrors( ) ) {
            std::cerr << p << std::endl;
            std::cerr << schema.errorHandler( ) << std::endl;
        }
        EXPECT_TRUE( schema.get( ) != nullptr );
    }
}

TEST_F( XmlSchemaTests, CtorInvalidSchemas ) {
    Dir::getInstance( )->chdir( Pathname{"tests/schemas/invalid"} );
    std::regex glob{".*\\.xsd"};
    auto ls = Dir::getInstance( )->read( ).entries( );
    std::vector<Pathname> schemaInvalidEntries{filter( ls, glob )};
    for ( Pathname &p : schemaInvalidEntries ) {
        std::ifstream f{p.toString( ), std::ios::in};
        ASSERT_TRUE( f.is_open( ) );
        const XmlDoc invalidSchema{f};
        XmlSchema schema;
        XmlSchemaParser validParser;
        invalidSchema >> validParser >> schema;
        EXPECT_TRUE( schema.errorHandler( ).hasErrors( ) );
        EXPECT_TRUE( schema.get( ) == nullptr );
    }
}

