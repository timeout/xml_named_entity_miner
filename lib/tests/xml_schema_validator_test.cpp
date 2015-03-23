#include "xml_schema_validator.hpp"
#include "xml_schema.hpp"
#include "xml_schema_parser.hpp"
#include "dir.hpp"

#include <fstream>
#include <array>

#include "gtest/gtest.h"
#include <limits.h>

static const Pathname buildDir_{Dir::getInstance( )->pwd( )};
static const std::array<std::string, 32> dirnames = {
    "001",    "002",    "003",    "004",     "005",     "006",    "007",    "008",
    "009",    "xsd001", "xsd002", "xsd003a", "xsd003b", "xsd004", "xsd005", "xsd006",
    "xsd007", "xsd008", "xsd009", "xsd010",  "xsd011",  "xsd012", "xsd013", "xsd014",
    "xsd015", "xsd016", "xsd017", "xsd018",  "xsd019",  "xsd020", "xsd021", "xsd022"};

class XmlSchemaValidatorTests : public testing::Test {
protected:
    virtual auto SetUp( ) -> void { Dir::getInstance( )->chdir( buildDir_ ); }
    auto populate( ) -> const std::vector<Pathname> & {
        Pathname dirname{buildDir_ + std::string{"tests/validation/sunData/combined"}};
        for ( auto subdir : dirnames ) {
            sunData_.push_back( Pathname{dirname} + subdir );
        }
        return sunData_;
    }

private:
    std::vector<Pathname> sunData_;
};

TEST_F( XmlSchemaValidatorTests, DefaultCtor ) {
    XmlSchemaValidator validator;
    EXPECT_FALSE( validator.errorHandler( ).hasErrors( ) );
}

TEST_F( XmlSchemaValidatorTests, Ctor ) {
    std::regex glob{R"(.*\.xsd)"};
    std::vector<Pathname> schemas;
    for ( auto p : populate( ) ) {
        auto ls = Dir::getInstance( )->chdir( p ).entries( );
        auto xsds = filter( ls, glob );
        schemas.insert( schemas.end( ), xsds.begin( ), xsds.end( ) );
    }
    for ( auto p : schemas ) {
        std::ifstream f{p.toString( ), std::ios::in};
        ASSERT_TRUE( f.is_open( ) );
        XmlDoc xml{f};
        XmlSchemaParser parser;
        XmlSchema schema;
        XmlSchemaValidator validator;
        xml >> parser >> schema >> validator;
        ASSERT_FALSE( xml.errorHandler( ).hasErrors( ) );
        ASSERT_FALSE( parser.errorHandler( ).hasErrors( ) );
        ASSERT_FALSE( schema.errorHandler( ).hasErrors( ) );
        EXPECT_FALSE( validator.errorHandler( ).hasErrors( ) );
    }
}

TEST_F( XmlSchemaValidatorTests, validate ) {
    std::regex xGlob{R"(.*[^e]\.xsd)"}; // no erronous xsds
    std::regex vGlob( R"(.*v\.xml)" );  // validates
    std::regex nGlob( R"(.*n\.xml)" );  // doesn't
    std::vector<Pathname> xsds, validXmls, nonValidXmls;
    unsigned count = 0;
    for ( auto p : populate( ) ) {
        auto ls = Dir::getInstance( )->chdir( p ).read( ).entries( );
        xsds = filter( ls, xGlob );
        validXmls = filter( ls, vGlob );
        nonValidXmls = filter( ls, nGlob );
        if ( !xsds.empty( ) ) {
            XmlSchemaParser parser;
            XmlSchema schema;
            XmlSchemaValidator validator;
            for ( auto fs : xsds ) {
                std::ifstream f{fs.toString( ), std::ios::in};
                XmlDoc xml{f};
                xml >> parser >> schema >> validator;
            }
            for ( auto fv : validXmls ) {
                std::ifstream f{fv.toString( ), std::ios::in};
                XmlDoc vXml{f};
                EXPECT_TRUE( validator.validate( vXml ) );
                if ( !validator.validate( vXml ) ) {
                    std::cerr << fv << std::endl;
                }
                ++count;
            }
            for ( auto fn : nonValidXmls ) {
                std::ifstream f{fn.toString( ), std::ios::in};
                XmlDoc nXml{f};
                EXPECT_FALSE( validator.validate( nXml ) );
                if ( validator.validate( nXml ) ) {
                    std::cerr << fn << std::endl;
                }
                ++count;
            }
        }
    }
    std::cerr << "tested: " << count << " files" << std::endl;
}

