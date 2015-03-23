#include "lib/xpath_ctxt.hpp"
#include "lib/xml_doc.hpp"

#include "gtest/gtest.h"
#include <limits.h>

#include "test_helper.hpp"

class XPathCtxtTest : public testing::Test {
protected:
    auto xmlBooksDoc( ) const -> const XmlDoc & { return creator.booksXml( ); }
    auto xmlMenuDoc( ) const -> const XmlDoc & { return creator.menuXml( ); }

private:
    XmlCreator creator;
};

TEST_F( XPathCtxtTest, DefaultCtor ) {
    XPathCtxt xpc;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, NullCtor ) {
    // empty xml
    const XmlDoc nullXml;
    XPathCtxt xpc{nullXml};
    ASSERT_EQ( false, testBool_f<XPathCtxt>( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, Ctor ) {
    XPathCtxt xpc{xmlBooksDoc( )};
    ASSERT_EQ( true, testBool_f<XPathCtxt>( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyCtor ) {
    // null XPathCtxt
    XPathCtxt nullXpc;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc ) );

    XPathCtxt cpNullXpc{nullXpc};
    ASSERT_EQ( false, testBool_f<XPathCtxt>( cpNullXpc ) );
    EXPECT_FALSE( cpNullXpc.errorHandler( ).hasErrors( ) );

    // XPathCtxt with null Xml
    const XmlDoc nullDoc;
    nullDoc >> nullXpc;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc ) );

    XPathCtxt cpNullDocXpc{nullXpc};
    ASSERT_EQ( false, testBool_f<XPathCtxt>( cpNullDocXpc ) );
    EXPECT_FALSE( cpNullDocXpc.errorHandler( ).hasErrors( ) );

    // XPathCtxt
    XPathCtxt xpc{xmlBooksDoc( )};
    ASSERT_EQ( true, testBool_f<XPathCtxt>( xpc ) );

    XPathCtxt cpXpc{xpc};
    ASSERT_EQ( true, testBool_f<XPathCtxt>( cpXpc ) );
    EXPECT_FALSE( cpXpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyAssignment ) {
    // assign null to null
    XPathCtxt nullXpc;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc ) );

    XPathCtxt nullXpc1;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc1 ) );

    nullXpc = nullXpc1;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc ) );
    EXPECT_FALSE( nullXpc.errorHandler( ).hasErrors( ) );

    // assign xpc to xpc
    XPathCtxt booksXpc{xmlBooksDoc( )};
    ASSERT_EQ( true, testBool_f<XPathCtxt>( booksXpc ) );

    XPathCtxt menuXpc{xmlMenuDoc( )};
    ASSERT_EQ( true, testBool_f<XPathCtxt>( menuXpc ) );

    booksXpc = menuXpc;
    ASSERT_EQ( true, testBool_f<XPathCtxt>( booksXpc ) );
    EXPECT_FALSE( booksXpc.errorHandler( ).hasErrors( ) );

    // assign null to xpc
    booksXpc = nullXpc;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( booksXpc ) );
    EXPECT_FALSE( booksXpc.errorHandler( ).hasErrors( ) );

    // assign xpc to null
    nullXpc1 = menuXpc;
    ASSERT_EQ( true, testBool_f<XPathCtxt>( nullXpc1 ) );
    EXPECT_FALSE( nullXpc1.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyMoveCtor ) {
    // null XPathCtxt
    XPathCtxt nullXpc;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc ) );

    XPathCtxt cpNullXpc{cp_f<XPathCtxt>( nullXpc )};
    ASSERT_EQ( false, testBool_f<XPathCtxt>( cpNullXpc ) );
    EXPECT_FALSE( cpNullXpc.errorHandler( ).hasErrors( ) );

    // XPathCtxt with null Xml
    const XmlDoc nullDoc;
    nullDoc >> nullXpc;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc ) );

    XPathCtxt cpNullDocXpc{cp_f<XPathCtxt>( nullXpc )};
    ASSERT_EQ( false, testBool_f<XPathCtxt>( cpNullDocXpc ) );
    EXPECT_FALSE( cpNullDocXpc.errorHandler( ).hasErrors( ) );

    // XPathCtxt
    XPathCtxt xpc{xmlBooksDoc( )};
    ASSERT_EQ( true, testBool_f<XPathCtxt>( xpc ) );

    XPathCtxt cpXpc{cp_f<XPathCtxt>( xpc )};
    ASSERT_EQ( true, testBool_f<XPathCtxt>( cpXpc ) );
    EXPECT_FALSE( cpXpc.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, CopyMoveAssignment ) {
    // assign null to null
    XPathCtxt nullXpc;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc ) );

    XPathCtxt nullXpc1;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc1 ) );

    nullXpc = cp_f<XPathCtxt>( nullXpc1 );
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc ) );
    EXPECT_FALSE( nullXpc.errorHandler( ).hasErrors( ) );

    // assign xpc to xpc
    XPathCtxt xpc{xmlBooksDoc( )};
    ASSERT_EQ( true, testBool_f<XPathCtxt>( xpc ) );

    XPathCtxt xpc1{xmlMenuDoc( )};
    ASSERT_EQ( true, testBool_f<XPathCtxt>( xpc1 ) );

    xpc = cp_f<XPathCtxt>( xpc1 );
    ASSERT_EQ( true, testBool_f<XPathCtxt>( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );

    // assign null to xpc
    xpc = cp_f<XPathCtxt>( nullXpc );
    ASSERT_EQ( false, testBool_f<XPathCtxt>( xpc ) );
    EXPECT_FALSE( xpc.errorHandler( ).hasErrors( ) );

    // assign xpc to null
    xpc1 = cp_f<XPathCtxt>( nullXpc1 );
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc1 ) );
    EXPECT_FALSE( nullXpc1.errorHandler( ).hasErrors( ) );
}

TEST_F( XPathCtxtTest, OperatorIn ) {
    XPathCtxt nullXpc;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc ) );

    XmlDoc xml;
    xml >> nullXpc;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc ) );

    xml = xmlBooksDoc( );
    xml >> nullXpc;
    ASSERT_EQ( true, testBool_f<XPathCtxt>( nullXpc ) );

    xml = xmlMenuDoc( );
    xml >> nullXpc;
    ASSERT_EQ( true, testBool_f<XPathCtxt>( nullXpc ) );

    XmlDoc nullDoc;
    nullDoc >> nullXpc;
    ASSERT_EQ( false, testBool_f<XPathCtxt>( nullXpc ) );
}
