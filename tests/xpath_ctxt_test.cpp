#include "xpath_ctxt.hpp"
#include "xml_doc.hpp"
#include "pathname.hpp"

#include <fstream>

#include "gtest/gtest.h"
#include <limits.h>

class XPathCtxtTest : public testing::Test {
protected:
    auto testable( const XPathCtxt &ctxt ) const -> bool { return ctxt ? true : false; }
    auto xml( const Pathname &path ) -> void {
        std::ifstream f{path.toString( ), std::ios::in};
        if ( !f.is_open( ) ) {
            std::cerr << "couldn't open: " << path.toString( ) << std::endl;
            ASSERT_FALSE( true );
        }
        xml_ = XmlDoc{f};
        if ( !xml_ ) {
            std::cerr << "xml empty" << std::endl;
            ASSERT_FALSE( true );
        }
        if ( xml_.errorHandler( ).hasErrors( ) ) {
            std::cerr << xml_.errorHandler( ) << std::endl;
            ASSERT_FALSE( true ); // burn baby, burn
        }
    }
    auto xml( ) const -> const XmlDoc & { return xml_; }
    auto f( XPathCtxt xpc ) -> XPathCtxt { return xpc; }

private:
    XmlDoc xml_;
};

TEST_F( XPathCtxtTest, DefaultCtor ) {
    XPathCtxt xpc;
    ASSERT_EQ( false, testable( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, NullCtor ) {
    // empty xml
    const XmlDoc nullXml;
    XPathCtxt xpc( nullXml );
    ASSERT_EQ( false, testable( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, Ctor ) {
    XPathCtxt xpc;
    xml( Pathname{"transformations/Jean Studer invalid.xml"} );
    xml( ) >> xpc;
    ASSERT_EQ( true, testable( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyCtor ) {
    XPathCtxt xpc;
    xml( Pathname{"transformations/Jean Studer invalid.xml"} );
    xml( ) >> xpc;
    XPathCtxt cp{xpc};
    ASSERT_EQ( true, testable( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );

    const XmlDoc nullXml;
    XPathCtxt nullXpc( nullXml );
    XPathCtxt nullXpcCp{nullXpc};
    ASSERT_EQ( false, testable( nullXpcCp ) );
    EXPECT_FALSE( nullXpcCp.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyAssignment ) {
    XPathCtxt xpc;
    xml( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    xml( ) >> xpc;

    XPathCtxt ass;
    xml( Pathname{"tests/xmltest/valid/sa/005.xml"} );
    xml( ) >> ass;
    ASSERT_EQ( true, testable( ass ) );
    EXPECT_FALSE( ass.errorHandler( ).hasErrors( ) );

    ass = xpc;
    ASSERT_EQ( true, testable( ass ) );
    EXPECT_FALSE( ass.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyMoveCtor ) {
    XPathCtxt c;
    xml( Pathname{"tests/xmltest/valid/sa/007.xml"} );
    xml( ) >> c;
    XPathCtxt mxpc{f( c )};
    ASSERT_EQ( true, testable( mxpc ) );
    EXPECT_FALSE( mxpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyMoveAssignment ) {
    XPathCtxt xpc;
    xml( Pathname{"tests/xmltest/valid/sa/001.xml"} );
    xml( ) >> xpc;

    XPathCtxt mass;
    xml( Pathname{"tests/xmltest/valid/sa/005.xml"} );
    xml( ) >> mass;

    mass = f( xpc );
    ASSERT_EQ( true, testable( mass ) );
    EXPECT_FALSE( mass.errorHandler( ).hasErrors( ) );
}
