#include "xpath_query.hpp"
#include "xpath_ctxt.hpp"
#include "xml_doc.hpp"

#include <fstream>

#include "gtest/gtest.h"
#include <limits.h>

class XPathQueryTest : public testing::Test {
protected:
    auto testable( const XPathQuery &query ) const -> bool {
        return query ? true : false;
    }
    auto xpc( const Pathname &path ) -> void {
        std::ifstream f{path.toString( ), std::ios::in};
        if ( !f.is_open( ) ) {
            std::cerr << "couldn't open: " << path.toString( ) << std::endl;
            ASSERT_FALSE( true );
        }
        XmlDoc xml{f};
        if ( !xml ) {
            std::cerr << "xml empty" << std::endl;
            ASSERT_FALSE( true );
        }
        if ( xml.errorHandler( ).hasErrors( ) ) {
            std::cerr << xml.errorHandler( ) << std::endl;
            ASSERT_FALSE( true ); // burn baby, burn
        }
        xml >> xpc_;
        if ( !xpc_ ) {
            std::cerr << "xpc empty" << std::endl;
            ASSERT_FALSE( true );
        }
    }
    auto xpc( ) const -> const XPathCtxt & { return xpc_; }
    auto f( XPathQuery query ) -> XPathQuery { return query; }

private:
    XPathCtxt xpc_;
};

TEST_F( XPathQueryTest, DefaultCtor ) {
    XPathQuery query;
    ASSERT_EQ( false, testable( query ) );
}

TEST_F( XPathQueryTest, Ctor ) {
    xpc( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    XPathCtxt xp{xpc( )};
    XPathQuery query1;
    xp >> query1;
    ASSERT_EQ( false, testable( query1 ) );

    XPathQuery query2( xp );
    ASSERT_EQ( false, testable( query2 ) );
}

TEST_F( XPathQueryTest, CopyCtor ) {
    XPathQuery query;
    XPathQuery queryCp{query};
    ASSERT_EQ( false, testable( query ) );

    xpc( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    XPathCtxt xp1{xpc( )};
    XPathQuery query1;
    xp1 >> query1;

    XPathQuery query2{query1};
    ASSERT_EQ( false, testable( query2 ) );
}

TEST_F( XPathQueryTest, CopyMoveCtor ) {
    XPathQuery query;
    XPathQuery queryCp{f( query )};
    ASSERT_EQ( false, testable( queryCp ) );

    xpc( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    XPathQuery query1;
    xpc( ) >> query1;
    XPathQuery query2{f( query1 )};
    ASSERT_EQ( false, testable( query2 ) );
    query2.query( "/element/leaf[1]" );
    ASSERT_EQ( true, testable( query2 ) );
    ASSERT_EQ( false, testable( query1 ) );
}

TEST_F( XPathQueryTest, CopyAssignment ) {
    xpc( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    XPathQuery query1;
    xpc( ) >> query1;

    xpc( Pathname{"tests/xmltest/valid/sa/008.xml"} );
    XPathCtxt xp2{xpc( )};
    XPathQuery query2{xp2};

    query1 = query2;
    ASSERT_EQ( false, testable( query1 ) );
    query1.query( "/element/leaf[1]" );
    ASSERT_EQ( true, testable( query1 ) );
}

TEST_F( XPathQueryTest, CopyMoveAssignment ) {
    xpc( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    XPathQuery query;
    xpc( ) >> query;
    query.query( "/element/sub" );

    xpc( Pathname{"tests/xmltest/valid/sa/006.xml"} );
    XPathQuery ass{xpc( )};
    ASSERT_EQ( false, testable( ass ) );
    ass = f( query );
    ASSERT_EQ( true, testable( ass ) );
}

TEST_F( XPathQueryTest, query ) {
    xpc( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    XPathCtxt xp{xpc( )};
    XPathQuery query1;
    xp >> query1;
    query1.query( "//element" );
    ASSERT_EQ( true, testable( query1 ) );
}
