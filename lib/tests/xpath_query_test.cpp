#include "xpath_query.hpp"

#include "gtest/gtest.h"
#include <limits.h>

#include "test_helper.hpp"

class XPathQueryTest : public testing::Test {
protected:
    auto xpcBooks( ) const -> const XPathCtxt & { return xpcCreator_.booksCtxt( ); }
    auto xpcMenu( ) const -> const XPathCtxt & { return xpcCreator_.menuCtxt( ); }

private:
    XPathCtxtCreator xpcCreator_;
};

TEST_F( XPathQueryTest, DefaultCtor ) {
    XPathQuery query;
    ASSERT_EQ( false, testBool_f<XPathQuery>( query ) );
}

TEST_F( XPathQueryTest, Ctor ) {
    XPathCtxt nullXpc;
    XPathQuery nullQuery{nullXpc};
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullQuery ) );

    XmlDoc nullDoc;
    nullDoc >> nullXpc;
    XPathQuery nullDocQuery{nullXpc};
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullDocQuery ) );

    XPathQuery booksQuery{xpcBooks( )};
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQuery ) );
}

TEST_F( XPathQueryTest, CopyCtor ) {
    XPathCtxt nullXpc;
    XPathQuery nullQuery{nullXpc};
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullQuery ) );

    XPathQuery cpNullQuery{nullQuery};
    ASSERT_EQ( false, testBool_f<XPathQuery>( cpNullQuery ) );

    XmlDoc nullDoc;
    nullDoc >> nullXpc;
    XPathQuery nullDocQuery{nullXpc};
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullDocQuery ) );

    XPathQuery cpNullDocQuery{nullQuery};
    ASSERT_EQ( false, testBool_f<XPathQuery>( cpNullDocQuery ) );

    XPathQuery booksQuery{xpcBooks( )};
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQuery ) );

    XPathQuery cpBooksQuery{booksQuery};
    ASSERT_EQ( false, testBool_f<XPathQuery>( cpBooksQuery ) );
}

TEST_F( XPathQueryTest, CopyMoveCtor ) {}

TEST_F( XPathQueryTest, CopyAssignment ) {
    // null to null
    XPathCtxt nullXpc;
    XPathQuery nullQueryLHS{nullXpc};
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullQueryLHS ) );

    XPathQuery nullQueryRHS;
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullQueryRHS ) );

    nullQueryLHS = nullQueryRHS;
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullQueryLHS ) );

    XmlDoc nullDoc;
    nullDoc >> nullXpc;
    XPathQuery nullDocQuery{nullXpc};
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullDocQuery ) );

    XPathQuery nullQueryLHS1;
    nullQueryLHS1 = nullDocQuery;
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullQueryLHS1 ) );

    // query to query
    XPathQuery booksQueryLHS{xpcBooks( )};
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQueryLHS ) );

    XPathQuery menuQueryRHS{xpcMenu( )};
    ASSERT_EQ( false, testBool_f<XPathQuery>( menuQueryRHS ) );

    booksQueryLHS = menuQueryRHS;
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQueryLHS ) );

    // null to query
    XPathQuery menuQueryLHS{xpcMenu( )};
    ASSERT_EQ( false, testBool_f<XPathQuery>( menuQueryLHS ) );

    menuQueryLHS = nullQueryRHS;
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQueryLHS ) );

    // query to null
    nullQueryLHS = menuQueryRHS;
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullQueryLHS ) );
}

TEST_F( XPathQueryTest, CopyMoveAssignment ) {}

TEST_F( XPathQueryTest, Query ) {
    // nullQuery
    XPathQuery nullQuery;
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullQuery ) );
    nullQuery.query( "//" );
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullQuery ) );

    XPathCtxt nullXpc;
    nullXpc >> nullQuery;
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullQuery ) );
    nullQuery.query( "//" );
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullQuery ) );

    XmlDoc nullDoc;
    nullDoc >> nullXpc;
    XPathQuery nullDocQuery{nullXpc};
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullDocQuery ) );
    nullDocQuery.query( "//" );
    ASSERT_EQ( false, testBool_f<XPathQuery>( nullDocQuery ) );

    // query doesn't exist, negative query
    XPathQuery booksQuery{xpcBooks( )};
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQuery ) );
    booksQuery.query( "//bad/query" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );

    // query doesn't exist, positive query
    XPathQuery menuQuery{xpcMenu( )};
    ASSERT_EQ( false, testBool_f<XPathQuery>( menuQuery ) );
    //!\\ the following query seg faults //!\\
    // menuQuery.query( R"(//)" );
    menuQuery.query( "//customer" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    // query exists, negative query
    menuQuery.query( "//bad/query" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    // query exists, positive query
    booksQuery.query( "//author" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );

    booksQuery.query( "//title" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );

    booksQuery.query( "//price" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );
}

TEST_F( XPathQueryTest, QueryCopyCtor ) {
    // query doesn't exist, negative query
    XPathQuery booksQuery{xpcBooks( )};
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQuery ) );
    booksQuery.query( "//bad/query" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );

    XPathQuery cpBooksQuery{booksQuery};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpBooksQuery ) );

    // query doesn't exist, positive query
    XPathQuery menuQuery{xpcMenu( )};
    ASSERT_EQ( false, testBool_f<XPathQuery>( menuQuery ) );
    //!\\ the following query seg faults //!\\
    // menuQuery.query( R"(//)" );
    menuQuery.query( "//customer" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    XPathQuery cpMenuQuery{menuQuery};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpMenuQuery ) );

    // positive query exists, positive query
    menuQuery.query( "//food" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    // query exists, negative query
    menuQuery.query( "//bad/query" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    // negative query exists, positive query
    booksQuery.query( "//author" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );
    XPathQuery cpPosBooksQuery{booksQuery};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpPosBooksQuery ) );
}

TEST_F( XPathQueryTest, QueryCopyAssignment ) {
    // query doesn't exist, negative query
    XPathQuery booksQuery{xpcBooks( )};
    booksQuery.query( "//bad/query" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );

    XPathQuery cpBooksQuery{booksQuery};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpBooksQuery ) );

    // query doesn't exist, positive query
    XPathQuery menuQuery{xpcMenu( )};
    //!\\ the following query seg faults //!\\
    // menuQuery.query( R"(//)" );
    menuQuery.query( "//customer" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    XPathQuery cpMenuQuery{menuQuery};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpMenuQuery ) );

    // positive query exists, positive query
    menuQuery.query( "//food" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    // query exists, negative query
    menuQuery.query( "//bad/query" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    // negative query exists, positive query
    booksQuery.query( "//author" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );
    XPathQuery cpPosBooksQuery{booksQuery};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpPosBooksQuery ) );

    XPathQuery nullQuery;
    XPathQuery cpNullQuery{nullQuery};
    ASSERT_EQ( false, testBool_f<XPathQuery>( cpNullQuery ) );

    // assign to null
    cpNullQuery = cpBooksQuery;
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpNullQuery ) );

    cpNullQuery = nullQuery;
    cpNullQuery = cpMenuQuery;
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpMenuQuery ) );

    // assign null to
    cpNullQuery = nullQuery;
    menuQuery = cpNullQuery;
    ASSERT_EQ( false, testBool_f<XPathQuery>( menuQuery ) );

    cpNullQuery = nullQuery;
    booksQuery = cpNullQuery;
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQuery ) );
}

TEST_F( XPathQueryTest, QueryCopyMoveCtor ) {
    // query doesn't exist, negative query
    XPathQuery booksQuery{xpcBooks()};
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQuery ) );
    booksQuery.query( "//bad/query" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );

    XPathQuery cpBooksQuery{cp_f<XPathQuery>( booksQuery )};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpBooksQuery ) );

    // query doesn't exist, positive query
    XPathQuery menuQuery{xpcMenu()};
    ASSERT_EQ( false, testBool_f<XPathQuery>( menuQuery ) );
    //!\\ the following query seg faults //!\\
    // menuQuery.query( R"(//)" );
    menuQuery.query( "//customer" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    XPathQuery cpMenuQuery{cp_f<XPathQuery>( menuQuery )};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpMenuQuery ) );

    // positive query exists, positive query
    menuQuery.query( "//food" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );
    XPathQuery cpPosMenuQuery{cp_f<XPathQuery>( menuQuery )};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpPosMenuQuery ) );

    // query exists, negative query
    menuQuery.query( "//bad/query" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );
    XPathQuery cpNegMenuQuery{cp_f<XPathQuery>( menuQuery )};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpNegMenuQuery ) );

    // negative query exists, positive query
    booksQuery.query( "//author" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );
    XPathQuery cpPosBooksQuery{cp_f<XPathQuery>( booksQuery )};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpPosBooksQuery ) );
}

TEST_F( XPathQueryTest, QueryCopyMoveAssignment ) {
    // query doesn't exist, negative query
    XPathQuery booksQuery{xpcBooks()};
    booksQuery.query( "//bad/query" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );

    XPathQuery cpBooksQuery{booksQuery};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpBooksQuery ) );

    // query doesn't exist, positive query
    XPathQuery menuQuery{xpcMenu()};
    //!\\ the following query seg faults //!\\
    // menuQuery.query( R"(//)" );
    menuQuery.query( "//customer" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    XPathQuery cpMenuQuery{menuQuery};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpMenuQuery ) );

    // positive query exists, positive query
    menuQuery.query( "//food" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    // query exists, negative query
    menuQuery.query( "//bad/query" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( menuQuery ) );

    // negative query exists, positive query
    booksQuery.query( "//author" );
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );
    XPathQuery cpPosBooksQuery{booksQuery};
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpPosBooksQuery ) );

    XPathQuery nullQuery;
    XPathQuery cpNullQuery{nullQuery};
    ASSERT_EQ( false, testBool_f<XPathQuery>( cpNullQuery ) );

    // assign to null
    cpNullQuery = cp_f<XPathQuery>( cpBooksQuery );
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpNullQuery ) );

    cpNullQuery = nullQuery;
    cpNullQuery = cp_f<XPathQuery>( cpMenuQuery );
    ASSERT_EQ( true, testBool_f<XPathQuery>( cpMenuQuery ) );

    // assign null to
    cpNullQuery = nullQuery;
    menuQuery = cp_f<XPathQuery>( cpNullQuery );
    ASSERT_EQ( false, testBool_f<XPathQuery>( menuQuery ) );

    cpNullQuery = nullQuery;
    booksQuery = cp_f<XPathQuery>( cpNullQuery );
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQuery ) );
}

TEST_F( XPathQueryTest, Queries ) {
    XPathQuery booksQuery{xpcBooks()};
    booksQuery.query( "//" );
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQuery ) );

    booksQuery.query("//*");
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );

    booksQuery.query("//author");
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );

    booksQuery.query("/");
    ASSERT_EQ( true, testBool_f<XPathQuery>( booksQuery ) );

    booksQuery.query("\\");
    ASSERT_EQ( false, testBool_f<XPathQuery>( booksQuery ) );
}
