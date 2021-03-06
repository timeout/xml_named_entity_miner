#include "lib/xpath_result_set.hpp"
#include "lib/xpath_query.hpp"
#include "lib/xpath_ctxt.hpp"

#include "gtest/gtest.h"
#include <limits.h>

#include "test_helper.hpp"

#include <iostream>
#include <cstddef>

class XPathResultSetTest : public testing::Test {
protected:
    auto queryBooks( ) const -> const XPathQuery & { return queryCreator_.booksQuery( ); }
    auto queryMenu( ) const -> const XPathQuery & { return queryCreator_.menuQuery( ); }

private:
    XPathQueryCreator queryCreator_;
};

TEST_F( XPathResultSetTest, DefaultCtor ) {
    XPathResultSet resultSet;
    EXPECT_EQ( 0, static_cast<const int>( resultSet.size( ) ) );
}

TEST_F( XPathResultSetTest, Ctor ) {
    XmlDoc nullXml;
    XPathCtxt nullXpc{nullXml};
    XPathQuery nullQuery{nullXpc};
    XPathResultSet nullResultSet{nullQuery};
    EXPECT_EQ( 0, static_cast<const int>( nullResultSet.size( ) ) );

    XPathQuery menuQuery{queryMenu( )};
    menuQuery.query( "//calories" );
    XPathResultSet menuResultSet{menuQuery};
    EXPECT_EQ( 5, static_cast<const int>( menuResultSet.size( ) ) );

    XPathQuery booksQuery{queryBooks( )};
    booksQuery.query( "//bad/query" );
    XPathResultSet booksResultSet{booksQuery};
    EXPECT_EQ( 0, static_cast<const int>(booksResultSet.size( ) ));
}

TEST_F( XPathResultSetTest, CopyCtor ) {
    XmlDoc nullXml;
    XPathCtxt nullXpc{nullXml};
    XPathQuery nullQuery{nullXpc};
    XPathResultSet nullResultSet{nullQuery};
    ASSERT_EQ( 0, static_cast<const int>(nullResultSet.size( ) ));

    XPathResultSet cpNullResultSet{nullResultSet};
    EXPECT_EQ( 0, static_cast<const int>(cpNullResultSet.size( ) ));

    XPathQuery menuQuery{queryMenu( )};
    menuQuery.query( "//calories" );
    XPathResultSet menuResultSet{menuQuery};
    ASSERT_EQ( 5, static_cast<const int>(menuResultSet.size( ) ));

    XPathResultSet cpMenuResultSet{menuResultSet};
    EXPECT_EQ( 5, static_cast<const int>(cpMenuResultSet.size( ) ));

    XPathQuery booksQuery{queryBooks( )};
    booksQuery.query( "//bad/query" );
    XPathResultSet booksResultSet{booksQuery};
    ASSERT_EQ( 0, static_cast<const int>(booksResultSet.size( ) ));

    XPathResultSet cpBooksResultSet{booksResultSet};
    EXPECT_EQ( 0, static_cast<const int>(cpBooksResultSet.size( ) ));
}

TEST_F( XPathResultSetTest, CopyMoveCtor ) {
    XPathCtxt nullXpc{XmlDoc{}};
    XPathQuery nullQuery{nullXpc};
    XPathResultSet nullResultSet{nullQuery};
    ASSERT_EQ( 0, static_cast<const int>(nullResultSet.size( ) ));

    XPathResultSet mvCpNullResultSet{cp_f<XPathResultSet>( nullResultSet )};
    EXPECT_EQ( 0, static_cast<const int>(mvCpNullResultSet.size( ) ));

    XPathQuery menuQuery{queryMenu( )};
    menuQuery.query( "//calories" );
    XPathResultSet menuResultSet{menuQuery};
    ASSERT_EQ( 5, static_cast<const int>(menuResultSet.size( ) ));

    XPathResultSet mvCpMenuResultSet{cp_f<XPathResultSet>( menuResultSet )};
    EXPECT_EQ( 5, static_cast<const int>(mvCpMenuResultSet.size( ) ));

    XPathQuery booksQuery{queryBooks( )};
    booksQuery.query( "//bad/query" );
    XPathResultSet booksResultSet{booksQuery};
    ASSERT_EQ( 0, static_cast<const int>(booksResultSet.size( ) ));

    XPathResultSet mvCpBooksResultSet{cp_f<XPathResultSet>( booksResultSet )};
    EXPECT_EQ( 0, static_cast<const int>(mvCpBooksResultSet.size( ) ));
}

TEST_F( XPathResultSetTest, CopyAssignment ) {
    // assign null to null
    XmlDoc nullXml;
    XPathCtxt nullXpc{nullXml};
    XPathQuery nullQuery{nullXpc};
    XPathResultSet nullResultSet{nullQuery};
    ASSERT_EQ( 0, static_cast<const int>(nullResultSet.size( ) ));

    XPathQuery defaultQuery;
    XPathResultSet nullResultSet1{defaultQuery};
    ASSERT_EQ( 0, static_cast<const int>(nullResultSet1.size( ) ));

    nullResultSet = nullResultSet1;
    ASSERT_EQ( 0, static_cast<const int>(nullResultSet.size( ) ));

    // assign to null
    XPathQuery menuQuery{queryMenu( )};
    menuQuery.query( "//calories" );
    XPathResultSet menuResultSet{menuQuery};
    ASSERT_EQ( 5, static_cast<const int>(menuResultSet.size( ) ));

    XPathResultSet cpNullResultSet1{nullResultSet};
    ASSERT_EQ( 0, static_cast<const int>(cpNullResultSet1.size( ) ));

    cpNullResultSet1 = menuResultSet;
    EXPECT_EQ( 5, static_cast<const int>(cpNullResultSet1.size( ) ));
    EXPECT_EQ( 5, static_cast<const int>(menuResultSet.size( ) ));

    XPathQuery booksQuery{queryBooks( )};
    booksQuery.query( "//bad/query" );
    XPathResultSet booksResultSet{booksQuery};
    ASSERT_EQ( 0, static_cast<const int>(booksResultSet.size( ) ));

    XPathResultSet cpNullResultSet2{nullResultSet};
    ASSERT_EQ( 0, static_cast<const int>(cpNullResultSet2.size( ) ));

    cpNullResultSet2 = booksResultSet;
    EXPECT_EQ( 0, static_cast<const int>(cpNullResultSet2.size( ) ));
    EXPECT_EQ( 0, static_cast<const int>(booksResultSet.size( ) ));

    // assign null
    ASSERT_EQ( 5, static_cast<const int>(cpNullResultSet1.size( ) ));
    cpNullResultSet1 = nullResultSet;
    EXPECT_EQ( 0, static_cast<const int>(cpNullResultSet1.size( ) ));
    EXPECT_EQ( 0, static_cast<const int>(nullResultSet.size( ) ));

    // assign
    // booksQuery.query( "//title" );
    XPathQuery booksQuery1{queryBooks( )};
    booksQuery1.query( "//title" );
    XPathResultSet booksResultSet1{booksQuery1};
    ASSERT_EQ( 12, static_cast<const int>(booksResultSet1.size( ) ));
    ASSERT_EQ( 5, static_cast<const int>(menuResultSet.size( ) ));

    XPathQuery menuQuery1{queryMenu( )};
    menuQuery1.query( "//calories" );
    XPathResultSet menuResultSet1{menuQuery1};
    ASSERT_EQ( 5, static_cast<const int>(menuResultSet1.size( ) ));

    menuResultSet1 = booksResultSet1;
    EXPECT_EQ( 12, static_cast<const int>(menuResultSet1.size( ) ));
    EXPECT_EQ( 12, static_cast<const int>(booksResultSet1.size( ) ));
}

TEST_F( XPathResultSetTest, CopyMoveAssignment ) {
    // assign null to null
    XmlDoc nullXml;
    XPathCtxt nullXpc{nullXml};
    XPathQuery nullQuery{nullXpc};
    XPathResultSet nullResultSet{nullQuery};
    ASSERT_EQ( 0, static_cast<const int>(nullResultSet.size( ) ));

    XPathQuery defaultQuery;
    XPathResultSet nullResultSet1{defaultQuery};
    ASSERT_EQ( 0, static_cast<const int>(nullResultSet1.size( ) ));

    nullResultSet = cp_f<XPathResultSet>( nullResultSet1 );
    ASSERT_EQ( 0, static_cast<const int>(nullResultSet.size( ) ));

    // assign to null
    XPathQuery menuQuery{queryMenu( )};
    menuQuery.query( "//calories" );
    XPathResultSet menuResultSet{menuQuery};
    ASSERT_EQ( 5, static_cast<const int>(menuResultSet.size( ) ));

    XPathResultSet cpNullResultSet1{nullResultSet};
    ASSERT_EQ( 0, static_cast<const int>(cpNullResultSet1.size( ) ));

    cpNullResultSet1 = cp_f<XPathResultSet>( menuResultSet );
    EXPECT_EQ( 5, static_cast<const int>(cpNullResultSet1.size( ) ));
    EXPECT_EQ( 5, static_cast<const int>(menuResultSet.size( ) ));

    XPathQuery booksQuery{queryBooks( )};
    booksQuery.query( "//bad/query" );
    XPathResultSet booksResultSet{booksQuery};
    ASSERT_EQ( 0, static_cast<const int>(booksResultSet.size( ) ));

    XPathResultSet cpNullResultSet2{nullResultSet};
    ASSERT_EQ( 0, static_cast<const int>(cpNullResultSet2.size( ) ));

    cpNullResultSet2 = cp_f<XPathResultSet>( booksResultSet );
    EXPECT_EQ( 0, static_cast<const int>(cpNullResultSet2.size( ) ));
    EXPECT_EQ( 0, static_cast<const int>(booksResultSet.size( ) ));

    // assign null
    ASSERT_EQ( 5, static_cast<const int>(cpNullResultSet1.size( ) ));
    cpNullResultSet1 = cp_f<XPathResultSet>( nullResultSet );
    EXPECT_EQ( 0, static_cast<const int>(cpNullResultSet1.size( ) ));
    EXPECT_EQ( 0, static_cast<const int>(nullResultSet.size( ) ));

    // assign
    XPathQuery booksQuery1{queryBooks( )};
    booksQuery1.query( "//title" );
    XPathResultSet booksResultSet1{booksQuery1};
    ASSERT_EQ( 12, static_cast<const int>(booksResultSet1.size( ) ));
    ASSERT_EQ( 5, static_cast<const int>(menuResultSet.size( ) ));

    XPathQuery menuQuery1{queryMenu( )};
    menuQuery1.query( "//calories" );
    XPathResultSet menuResultSet1{menuQuery1};
    ASSERT_EQ( 5, static_cast<const int>(menuResultSet1.size( ) ));

    menuResultSet1 = cp_f<XPathResultSet>( booksResultSet1 );
    EXPECT_EQ( 12, static_cast<const int>(menuResultSet1.size( ) ));
    EXPECT_EQ( 12, static_cast<const int>(booksResultSet1.size( ) ));
}

TEST_F( XPathResultSetTest, IteratorTest ) {
    XPathQuery menuQuery{queryMenu( )};
    menuQuery.query( "//calories" );
    XPathResultSet menuResultSet{menuQuery};
    auto iter = menuResultSet.begin( );
    for ( ; iter != menuResultSet.end( ); ++iter ) {
        std::cerr << ( *iter ).content( ) << std::endl;
    }
    // auto el = menuResultSet[0];
    // std::cerr << el.content( ) << std::endl;

    XPathQuery menuQuery1{queryMenu( )};
    menuQuery1.query( "//description" );
    XPathResultSet menuResultSet1{menuQuery1};
    for ( auto it = menuResultSet1.begin( ); it != menuResultSet1.end( ); ++it ) {
        std::cerr << ( *it ).content( ) << std::endl;
    }

    XPathQuery menuQuery2{queryMenu( )};
    menuQuery2.query( "//bad_query" );
    XPathResultSet menuResultSet2{menuQuery2};
    for ( auto it = menuResultSet2.begin( ); it != menuResultSet2.end( ); ++it ) {
        std::cerr << "result: '" << ( *it ).content( ) << "'" << std::endl;
    }
    iter = menuResultSet.begin( );
    std::cerr << "accessing iterator[ 2 ]: " << iter[2].content( ) << std::endl;
    std::cerr << "decrement iterator: " << ( *--iter ).content( ) << std::endl;
    std::cerr << "decrement iterator: " << ( *--iter ).content( ) << std::endl;
    std::cerr << "post decrement iterator: " << ( *iter-- ).content( ) << std::endl;
}
