#include "xpath_result_set.hpp"
#include "xpath_query.hpp"
#include "xpath_ctxt.hpp"

#include "gtest/gtest.h"
#include <limits.h>

#include "test_helper.hpp"

class XPathResultSetTest : public testing::Test {
protected:
    auto queryBooks( ) const -> const XPathQuery & { return queryCreator_.booksQuery( ); }
    auto queryMenu( ) const -> const XPathQuery & { return queryCreator_.menuQuery( ); }

private:
    XPathQueryCreator queryCreator_;
};

TEST_F( XPathResultSetTest, DefaultCtor ) {
    XPathResultSet resultSet;
    EXPECT_EQ( 0, resultSet.size( ) );
}

TEST_F( XPathResultSetTest, Ctor ) {
    XmlDoc nullXml;
    XPathCtxt nullXpc{nullXml};
    XPathQuery nullQuery{nullXpc};
    XPathResultSet nullResultSet{nullQuery};
    EXPECT_EQ( 0, nullResultSet.size( ) );

    XPathQuery menuQuery{queryMenu( )};
    menuQuery.query( "//calories" );
    XPathResultSet menuResultSet{menuQuery};
    EXPECT_EQ( 5, menuResultSet.size( ) );

    XPathQuery booksQuery{queryBooks( )};
    booksQuery.query( "//bad/query" );
    XPathResultSet booksResultSet{booksQuery};
    EXPECT_EQ( 0, booksResultSet.size( ) );
}

TEST_F( XPathResultSetTest, CopyCtor ) {
    XmlDoc nullXml;
    XPathCtxt nullXpc{nullXml};
    XPathQuery nullQuery{nullXpc};
    XPathResultSet nullResultSet{nullQuery};
    ASSERT_EQ( 0, nullResultSet.size( ) );

    XPathResultSet cpNullResultSet{nullResultSet};
    EXPECT_EQ( 0, cpNullResultSet.size( ) );

    XPathQuery menuQuery{queryMenu( )};
    menuQuery.query( "//calories" );
    XPathResultSet menuResultSet{menuQuery};
    ASSERT_EQ( 5, menuResultSet.size( ) );

    XPathResultSet cpMenuResultSet{menuResultSet};
    EXPECT_EQ( 5, cpMenuResultSet.size( ) );

    XPathQuery booksQuery{queryBooks( )};
    booksQuery.query( "//bad/query" );
    XPathResultSet booksResultSet{booksQuery};
    ASSERT_EQ( 0, booksResultSet.size( ) );

    XPathResultSet cpBooksResultSet{booksResultSet};
    EXPECT_EQ( 0, cpBooksResultSet.size( ) );
}

TEST_F( XPathResultSetTest, CopyMoveCtor ) {
    XPathCtxt nullXpc{XmlDoc{}};
    XPathQuery nullQuery{nullXpc};
    XPathResultSet nullResultSet{nullQuery};
    ASSERT_EQ( 0, nullResultSet.size( ) );

    XPathResultSet mvCpNullResultSet{cp_f<XPathResultSet>( nullResultSet )};
    EXPECT_EQ( 0, mvCpNullResultSet.size( ) );

    XPathQuery menuQuery{queryMenu()};
    menuQuery.query( "//calories" );
    XPathResultSet menuResultSet{menuQuery};
    ASSERT_EQ( 5, menuResultSet.size( ) );

    XPathResultSet mvCpMenuResultSet{cp_f<XPathResultSet>( menuResultSet )};
    EXPECT_EQ( 5, mvCpMenuResultSet.size( ) );

    XPathQuery booksQuery{queryBooks()};
    booksQuery.query( "//bad/query" );
    XPathResultSet booksResultSet{booksQuery};
    ASSERT_EQ( 0, booksResultSet.size( ) );

    XPathResultSet mvCpBooksResultSet{cp_f<XPathResultSet>( booksResultSet )};
    EXPECT_EQ( 0, mvCpBooksResultSet.size( ) );
}

TEST_F( XPathResultSetTest, CopyAssignment ) {
    // assign null to null
    XmlDoc nullXml;
    XPathCtxt nullXpc{nullXml};
    XPathQuery nullQuery{nullXpc};
    XPathResultSet nullResultSet{nullQuery};
    ASSERT_EQ( 0, nullResultSet.size( ) );

    XPathQuery defaultQuery;
    XPathResultSet nullResultSet1{defaultQuery};
    ASSERT_EQ( 0, nullResultSet1.size( ) );

    nullResultSet = nullResultSet1;
    ASSERT_EQ( 0, nullResultSet.size( ) );

    // assign to null
    XPathQuery menuQuery{queryMenu()};
    menuQuery.query( "//calories" );
    XPathResultSet menuResultSet{menuQuery};
    ASSERT_EQ( 5, menuResultSet.size( ) );

    XPathResultSet cpNullResultSet1{nullResultSet};
    ASSERT_EQ( 0, cpNullResultSet1.size( ) );

    cpNullResultSet1 = menuResultSet;
    EXPECT_EQ( 5, cpNullResultSet1.size( ) );
    EXPECT_EQ( 5, menuResultSet.size( ) );

    XPathQuery booksQuery{queryBooks()};
    booksQuery.query( "//bad/query" );
    XPathResultSet booksResultSet{booksQuery};
    ASSERT_EQ( 0, booksResultSet.size( ) );

    XPathResultSet cpNullResultSet2{nullResultSet};
    ASSERT_EQ( 0, cpNullResultSet2.size( ) );

    cpNullResultSet2 = booksResultSet;
    EXPECT_EQ( 0, cpNullResultSet2.size( ) );
    EXPECT_EQ( 0, booksResultSet.size( ) );

    // assign null
    ASSERT_EQ( 5, cpNullResultSet1.size( ) );
    cpNullResultSet1 = nullResultSet;
    EXPECT_EQ( 0, cpNullResultSet1.size( ) );
    EXPECT_EQ( 0, nullResultSet.size( ) );

    // assign
    // booksQuery.query( "//title" );
    XPathQuery booksQuery1{queryBooks()};
    booksQuery1.query( "//title" );
    XPathResultSet booksResultSet1{booksQuery1};
    ASSERT_EQ( 12, booksResultSet1.size( ) );
    ASSERT_EQ( 5, menuResultSet.size( ) );

    XPathQuery menuQuery1{queryMenu()};
    menuQuery1.query( "//calories" );
    XPathResultSet menuResultSet1{menuQuery1};
    ASSERT_EQ( 5, menuResultSet1.size( ) );

    menuResultSet1 = booksResultSet1;
    EXPECT_EQ( 12, menuResultSet1.size( ) );
    EXPECT_EQ( 12, booksResultSet1.size( ) );
}

TEST_F( XPathResultSetTest, CopyMoveAssignment ) {
    // assign null to null
    XmlDoc nullXml;
    XPathCtxt nullXpc{nullXml};
    XPathQuery nullQuery{nullXpc};
    XPathResultSet nullResultSet{nullQuery};
    ASSERT_EQ( 0, nullResultSet.size( ) );

    XPathQuery defaultQuery;
    XPathResultSet nullResultSet1{defaultQuery};
    ASSERT_EQ( 0, nullResultSet1.size( ) );

    nullResultSet = cp_f<XPathResultSet>( nullResultSet1 );
    ASSERT_EQ( 0, nullResultSet.size( ) );

    // assign to null
    XPathQuery menuQuery{queryMenu()};
    menuQuery.query( "//calories" );
    XPathResultSet menuResultSet{menuQuery};
    ASSERT_EQ( 5, menuResultSet.size( ) );

    XPathResultSet cpNullResultSet1{nullResultSet};
    ASSERT_EQ( 0, cpNullResultSet1.size( ) );

    cpNullResultSet1 = cp_f<XPathResultSet>( menuResultSet );
    EXPECT_EQ( 5, cpNullResultSet1.size( ) );
    EXPECT_EQ( 5, menuResultSet.size( ) );

    XPathQuery booksQuery{queryBooks()};
    booksQuery.query( "//bad/query" );
    XPathResultSet booksResultSet{booksQuery};
    ASSERT_EQ( 0, booksResultSet.size( ) );

    XPathResultSet cpNullResultSet2{nullResultSet};
    ASSERT_EQ( 0, cpNullResultSet2.size( ) );

    cpNullResultSet2 = cp_f<XPathResultSet>( booksResultSet );
    EXPECT_EQ( 0, cpNullResultSet2.size( ) );
    EXPECT_EQ( 0, booksResultSet.size( ) );

    // assign null
    ASSERT_EQ( 5, cpNullResultSet1.size( ) );
    cpNullResultSet1 = cp_f<XPathResultSet>( nullResultSet );
    EXPECT_EQ( 0, cpNullResultSet1.size( ) );
    EXPECT_EQ( 0, nullResultSet.size( ) );

    // assign
    XPathQuery booksQuery1{queryBooks()};
    booksQuery1.query( "//title" );
    XPathResultSet booksResultSet1{booksQuery1};
    ASSERT_EQ( 12, booksResultSet1.size( ) );
    ASSERT_EQ( 5, menuResultSet.size( ) );

    XPathQuery menuQuery1{queryMenu()};
    menuQuery1.query( "//calories" );
    XPathResultSet menuResultSet1{menuQuery1};
    ASSERT_EQ( 5, menuResultSet1.size( ) );

    menuResultSet1 = cp_f<XPathResultSet>( booksResultSet1 );
    EXPECT_EQ( 12, menuResultSet1.size( ) );
    EXPECT_EQ( 12, booksResultSet1.size( ) );
}

